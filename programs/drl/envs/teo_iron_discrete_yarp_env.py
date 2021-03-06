################### GENERAL ##################################
import numpy as np
import time
import copy

################### RLLAB ####################################
from .base import Env
from rllab.spaces import Discrete
from rllab.envs.base import Step
from rllab.core.serializable import Serializable
from rllab.spaces import Box

################### YARP #####################################
import yarp
import kinematics_dynamics

################### FAST-DTW #################################
from scipy.spatial.distance import euclidean
from fastdtw import fastdtw


class TeoIronDiscreteYarpEnv(Env, Serializable):

    ################### INIT ###############################

    def __init__(self):

        self.yarpDelay=0.1
        self.yarpForceDelay=0.5

        self.action_inc=5
        self.num_step=0
        self.percentage=0

        ################ YARP ###############################################
        yarp.Network.init()

        if yarp.Network.checkNetwork() != True:  # let's see if there was actually a reachable YARP network
            print('[error] Please try running yarp server')  # tell the user to start one with 'yarp server' if there isn't any
            quit()

        mentalOptions = yarp.Property()
        mentalOptions.put("device", "controlboardwrapper2")  # device
        mentalOptions.put("subdevice", "YarpOpenraveControlboard")  # device
        mentalOptions.put("env", "/usr/local/share/xgnitive/contexts/models/teo_cgda_iros_iron.env.xml") #Environment
        mentalOptions.put("name", "/drl/rightArm")  # Teo
        mentalOptions.put("robotIndex", 0)  # Teo
        mentalOptions.put("manipulatorIndex", 2)  # Right_Arm
        mentalOptions.put("genRefSpeed", 9999999)  # Right_Arm

        ################ YARP CONNECT TO FORCE ###############################

        orPlugins = mentalOptions.addGroup("orPlugins")

        #print("************************************* HASTA AQUI LLEGUE 0************************************************************************************")

        orPlugin1 = orPlugins.addGroup("OpenraveYarpForceEstimator")  # Our lovely plugin (◕‿◕✿)
        orPlugin1.put("module", "OpenraveYarpForceEstimator")
        orPlugin1.put("commands", "open --name /drl/openraveYarpForceEstimator/rpc:s --wrinkleSize 4")

        # define Device
        self.mentalDevice = yarp.PolyDriver()
        # open device
        self.mentalDevice.open(mentalOptions)
        if not self.mentalDevice.isValid():
            print("Mental robot device not available.\n")
            self.mentalDevice.close()
            yarp.Network.fini()
            return 1

        '''

        # Read state
        self.cmd = yarp.Bottle()
        self.res = yarp.Bottle()

        remotePaint = "/drl/openraveYarpPaintSquares/rpc:s"
        localPaint = "/cgda/openraveYarpPaintSquares/rpc:c"

        self.rpcClient = yarp.RpcClient()
        self.rpcClient.open(localPaint)  # Connect to local paint
        '''


        #self.forcePort= yarp.BufferedPortBottle()
        #self.forcePort.open("/force:i")

        remoteForce = "/drl/openraveYarpForceEstimator/rpc:s"
        localForce = "/cgda/openraveYarpForceEstimator/rpc:c"

        self.rpcClient = yarp.RpcClient()
        self.rpcClient.open(localForce)  # Connect to local force

        while True:  ##do-while
            yarp.Network.connect(localForce, remoteForce)
            print("Wait to connect to force server...\n")
            time.sleep(self.yarpDelay)
            if self.rpcClient.getOutputCount() != 0:
                break

        '''
        #No needed with the new iron setup
        ################ YARP CONNECT TO CARTESIAN ###############################

        solverOptions = yarp.Property()
        solverOptions.put("device","KdlSolver")
        solverOptions.put("kinematics", "/usr/local/share/teo/contexts/kinematics/rightArmKinematics-pan45-tilt30.ini")

        # define Device
        self.solverDevice = yarp.PolyDriver()
        # open device
        self.solverDevice.open(solverOptions)
        if not self.solverDevice.isValid():
            print("Solver robot device not available.\n")
            self.solverDevice.close()
            yarp.Network.fini()
            return 1

        
        print("yeah------------------------------1")
        self.cartesianSolver = kinematics_dynamics.viewICartesianSolver(self.solverDevice)
        #x = yarp.DVector()
        #stat = self.cartesianControl.stat(x)
        #print ('<', yarp.Vocab.decode(stat), '[%s]' % ', '.join(map(str, x)))
        q = yarp.DVector(6,0)  # std::vector<double> sadasd(size,value)
        x = yarp.DVector()
        self.cartesianSolver.fwdKin(q,x)
        print('< [%s]' % ', '.join(map(str, x)))
        print("yeag------------------------------2")
        '''

        ################ YARP CONNECT TO ENCODERS ############################

        self.rpcClientCart = yarp.RpcClient()
        self.rpcClientCart.open("/encs:c")
        while True:
            yarp.Network.connect("/encs:c", "/teoSim/rightArm/rpc:i")
            print("Wait to connect to rightArm server...\n")
            # time.sleep(self.yarpDelay)
            if self.rpcClientCart.getOutputCount() != 0:
                break

        ################ YARP OBTAIN LIMITS ###############################
        #self.mentalPositionControl = self.mentalDevice.viewIControlLimits()
        self.mentalControlLimits = self.mentalDevice.viewIControlLimits()

        self.min0 = yarp.DVector(1)
        self.max0 = yarp.DVector(1)
        self.mentalControlLimits.getLimits(0, self.min0, self.max0)

        self.min1 = yarp.DVector(1)
        self.max1 = yarp.DVector(1)
        self.mentalControlLimits.getLimits(1, self.min1, self.max1)

        self.min3 = yarp.DVector(1)
        self.max3 = yarp.DVector(1)
        self.mentalControlLimits.getLimits(3, self.min3, self.max3)

        #self.lbound=-15
        #self.ubound=100

        ############### GENERAL ############################################

        #
        Serializable.quick_init(self, locals())

        #Read state
        self.cmd= yarp.Bottle()
        self.res= yarp.Bottle()

        #Position Control
        self.mentalPositionControl = self.mentalDevice.viewIPositionControl()

        #Define Trajectories [x,y,z,Fz]:
        self.attemp=np.array([0,0,0,0])

        self.goal=np.array([[0.272805, -0.500201, 0.012808, 5.775318],
                           [0.272620, -0.502092, 0.012907, 5.918067],
                           [0.266961, -0.508060, -0.000334, 8.253265],
                           [0.251811, -0.514240, -0.034490, 15.243059],
                           [0.238970, -0.514431, -0.067436, -6.968969],
                           [0.275419, -0.523437, -0.045262, -30.054635],
                           [0.319814, -0.541889, -0.003238, -11.918344],
                           [0.334939, -0.561616, 0.015075, -1.851854],
                           [0.340740, -0.560490, 0.019368, -1.511597]])

        self.start_state = [0,0,0]
        self.state = None
        self.domain_fig = None
        self.reward=0

    ################### RESET #############################

    def reset(self):
        self.state = copy.deepcopy(self.start_state) #rmbr

        #Set initial position:
        dEncRaw = np.empty(6, float)

        dEncRaw[0] = 0
        dEncRaw[1] = 0
        dEncRaw[3] = 0
        
        #reset reward
        self.reward=0

        self.mentalPositionControl.positionMove(0, dEncRaw[0])
        self.mentalPositionControl.positionMove(1, dEncRaw[1])
        self.mentalPositionControl.positionMove(3, dEncRaw[3])

        print(self.state)
        time.sleep(self.yarpDelay)  # pause

        #Reset num_steps
        self.num_step = 0

        #Set percentage of painted wall as zero
        self.percentage = 0

        #reset wrinkle
        self.cmd.clear()
        self.res.clear()
        self.cmd.addString("reset")
        self.rpcClient.write(self.cmd, self.res)


        return self.state

    ################### ACTION DEFINITION #################

    @staticmethod
    def action_from_direction(d):
        """
        Return the action corresponding to the given direction. This is a helper method for debugging and testing
        purposes.
        :return: the action index corresponding to the given direction
        """
        return dict( #The actions for the system are:
            inc_0=0,
            dec_0=1,
            inc_1=2,
            dec_1=3,
            inc_3=4,
            dec_3=5
        )[d]

    ################### STEP ##############################

    def step(self, action):

        #Get possible states with the actual state.
        possible_next_states = self.get_possible_next_states(self.state, action)

        #Choose an action at random with the probabilities obtained in get possible states. This means off-policy
        probs = [x[1] for x in possible_next_states]
        next_state_idx = np.random.choice(len(probs), p=probs)
        next_state = possible_next_states[next_state_idx][0]

        #print("next state before move is", next_state)

        #Try to move the robot.

        #print("the next state is ",next_state)
        print("Me voy a mover a:", next_state[0],next_state[1],next_state[2])
        dEncRaw = np.empty(6,float)

        dEncRaw[0] = next_state[0]
        dEncRaw[1] = next_state[1]
        dEncRaw[3] = next_state[2]

        #The new state is equal to state where i moved.
        #self.state[0] = next_state[0]
        #self.state[1] = next_state[1]
        #self.state[2] = next_state[2]

        #dEncRaw[0] = 45
        #dEncRaw[1] = 0
        #dEncRaw[3] = 0

        self.mentalPositionControl.positionMove(0, dEncRaw[0])
        self.mentalPositionControl.positionMove(1, dEncRaw[1])
        self.mentalPositionControl.positionMove(3, dEncRaw[3])

        #Just some sleep for debug
        #print("Moving the robot")
        time.sleep(self.yarpDelay)  # pause

        ################ GET ENCODERS ##########################################

        #First obtain current position with Encoders.
        enc = self.mentalDevice.viewIEncoders()  # make an encoder controller object we call 'enc'
        axes = enc.getAxes()
        v = yarp.DVector(axes)  # create a YARP vector of doubles the size of the number of elements read by enc, call it 'v'
        enc.getEncoders(v)  # read the encoder values and put them into 'v'

        self.state = []
        self.state.append(v[0])
        self.state.append(v[1])
        self.state.append(v[3])

        '''
        ################ GET CARTESIAN POSITION #################################

        #IN THE CURRENT SETUP OF IRON THE CARTESIAN POSITION IS NOT NEEDED.

        self.cartesianSolver = kinematics_dynamics.viewICartesianSolver(self.solverDevice)
        #x = yarp.DVector()
        #stat = self.cartesianControl.stat(x)
        #print ('<', yarp.Vocab.decode(stat), '[%s]' % ', '.join(map(str, x)))
        #q = yarp.DVector(6,0)
        x = yarp.DVector()
        self.cartesianSolver.fwdKin(v,x)
        print('< [%s]' % ', '.join(map(str, x)))
        '''

        '''
        ################ GET FORCE ##############################################

        force = self.forcePort.read(False)

        #Waiting to receive force
        if not force:
            while(1):
                force = self.forcePort.read(False)
                print("No force received yet")
                time.sleep(self.yarpForceDelay)  # pause
                if b:
                    break

        print("La fuerza obtenida es: ", force.toString())
        
        '''



        '''
        self.cmd.clear()
        self.res.clear()
        self.cmd.addString("get encs")
        self.rpcClientCart.write(self.cmd, self.res)

        #print("La posición después de moverme es: ", self.res)
        print("Got after movement : ", self.res.toString())

        print("--------------------------------------------------------------\n")
        '''


        ################ YARP REWARD #################################
	
        '''
        #This idea is not currently used.
	
        # Idea reward: Executed trajectory - target trajectory. The Executed trajectory is init with 0s. It has to be comulative.
        # self.trajectory (??)
	
        print("x0 es: ",x[0])
        print("x1 es: ",x[1])
        print("x2 es: ",x[2])

        self.attemp=np.append(self.attemp, np.array([x[0],x[1],x[2],float(force.toString())]))

        print("Attemp is: ", self.attemp)
        print("Goal is: ", self.goal)

        distance, path = fastdtw(self.attemp, self.goal, dist=euclidean)

        print("The distance is: ", distance)

        reward=1/distance;

        if distance<=1:
            done=True
        else:
            done=False

        print ("El reward es: ", reward)
	'''

	################ YARP GET % IRONED #################################

        # self.cmd= "get"
        self.cmd.clear()
        self.res.clear()
        self.cmd.addString("get")
        self.rpcClient.write(self.cmd, self.res)

        # print("The size of print after get is: ", self.res.size())

        new_percentage = 0
        for i in range(self.res.size()):
            print("The size of res is : ", self.res.size())
            new_percentage = new_percentage + self.res.get(i).asInt()
            # print(i)

        #print ("The number of squares painted is: ", new_percentage)
        #print ("number of squares is: ", self.res.size())

        new_percentage = (new_percentage * 100.0 / self.res.size())

        self.reward = 0
        # If a paint additional wall reward=1
        if new_percentage > self.percentage:
            print("******************************************************************************************** painted :)")
            self.reward = +100000
            # quit()
        elif new_percentage < self.percentage:
            self.reward = -100000
        else:
            print("nothing ironed :(")
            # self.reward= self.reward #-self.num_step*self.action_penalty

        #self.num_step = self.num_step + 1

        self.percentage = new_percentage
        print ("Percentage ironed is: ", self.percentage)

        if self.percentage == 100:
            done = True
        else:
            done = False

        print ("El reward es: ", self.reward)

        '''

        self.res.clear()
        self.cmd.clear()
        self.cmd.addString("force")
        self.rpcClient.write(self.cmd, self.res)
        print(self.res.get(i).asInt())

        self.res.clear()
        self.cmd.clear()
        self.cmd.addString("iron 1 0 0 0")
        self.rpcClient.write(self.cmd, self.res)
        print(self.res.get(i).asInt())
        '''


        return Step(observation=self.state, reward=self.reward, done=done)


        #return Step(observation=self.state, reward=reward, done=done)

    ################### NEXT STATES #############################

    def get_possible_next_states(self, state, action):

        """
        Given the state and action, return a list of possible next states and their probabilities. Only next states
        with nonzero probabilities will be returned
        :param action: action
        :return: a list of pairs (s', p(s'|s,a))
        """

        # assert self.observation_space.contains(state)
        # assert self.action_space.contains(action)


        ################### GET FUTURE STATES #############################

        #Possible increments produced by the action:
        increments = np.array([[self.action_inc, 0, 0], [-self.action_inc, 0, 0], [0, self.action_inc, 0], [0, -self.action_inc, 0], [0, 0, self.action_inc], [0, 0, -self.action_inc]])

        next_state = np.clip(
            state + increments[action],
            [self.min0[0], self.min1[0], self.min3[0]], # Limits
            [self.max0[0], self.max1[0], self.max3[0]]  # Limits
        )

        print("the next state is ", next_state)

        return [(next_state, 1.)]

    ################### ACTION ##################################

    @property
    def action_space(self):
        return Discrete(6)

    ################### OBSERVATION #############################

    @property
    def observation_space(self):
        return Box(low=-np.inf, high=np.inf, shape=(3,)) #The state is defined as a 3D continous space
        #return Discrete(self.n_row * self.n_col * self.levels)
