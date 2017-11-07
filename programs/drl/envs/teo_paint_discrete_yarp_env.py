################### GENERAL ##############################
import numpy as np
import time
import copy

################### RLLAB ################################
from .base import Env
from rllab.spaces import Discrete
from rllab.envs.base import Step
from rllab.core.serializable import Serializable
from rllab.spaces import Box

################### YARP ##############################
import yarp

################### TO USE NOW ############################

################### ENVIRONMENT #############################

class TeoPaintDiscreteYarpEnv(Env, Serializable):
    """
    'S' : starting point
    'F' or '.': free space
    'W' or 'x': wall
    'H' or 'o': hole (terminates episode)
    'G' : goal
    """

    ################### INIT ###############################

    def __init__(self):

        self.yarpDelay=0.01
        self.action_inc=5
        self.action_penalty=0.1
        self.num_step=0
        self.percentage=0

        ################ YARP ###############################################
        yarp.Network.init()

        if yarp.Network.checkNetwork() != True:  # let's see if there was actually a reachable YARP network
            print('[error] Please try running yarp server')  # tell the user to start one with 'yarp server' if there isn't any
            quit()

        mentalOptions= yarp.Property()
        
        ################ YARP CONNECT TO MOVE ###############################
        # This device will have three different options, device, remote local.
        mentalOptions.put("device", "controlboardwrapper2")  # device
        mentalOptions.put("subdevice", "YarpOpenraveControlboard") #device
        mentalOptions.put("env", "/usr/local/share/xgnitive/contexts/models/teo_cgda_iros.env.xml") #Environment
        mentalOptions.put("name", "/drl/rightArm")  # Teo
        mentalOptions.put("robotIndex", 0) #Teo
        mentalOptions.put("manipulatorIndex", 2) #Right_Arm

        ################ YARP CONNECT TO PAINT ###############################
        orPlugins = mentalOptions.addGroup("orPlugins")

        orPlugin1 = orPlugins.addGroup("OpenraveYarpPaintSquares")  # Our lovely plugin (◕‿◕✿))
        orPlugin1.put("module", "OpenraveYarpPaintSquares")
        orPlugin1.put("commands", "open /drl/openraveYarpPaintSquares/rpc:s")

        # orPlugin2 = orPlugins.addGroup("OpenraveYarpPaintSquares2")  # Our lovely plugin (◕‿◕✿))
        # orPlugin2.put("module","OpenraveYarpPaintSquares2a")
        # orPlugin2.put("commands", "open")


        #remoteMental="/teoSim/rightArm"
        #mentalOptions.put("remote", remoteMental) #remote
        #localMental="/cgdaMental"
        #mentalOptions.put("local", localMental) #local

        #define Device
        #yarp.dev.Polydriver(mentalDevice)
        self.mentalDevice= yarp.PolyDriver()
        #open device
        self.mentalDevice.open(mentalOptions)
        if not self.mentalDevice.isValid() :
            print("Mental robot device not available.\n")
            self.mentalDevice.close()
            yarp.Network.fini()
            return 1

        print("Mental robot device available.\n")

        self.mentalPositionControl = self.mentalDevice.viewIPositionControl()
        self.mentalControlLimits = self.mentalDevice.viewIControlLimits()

        self.min0 = yarp.DVector(1)
        self.max0 = yarp.DVector(1)
        self.mentalControlLimits.getLimits(0, self.min0, self.max0)

        self.min1 = yarp.DVector(1)
        self.max1 = yarp.DVector(1)
        self.mentalControlLimits.getLimits(1, self.min1, self.max1)

        self.min3 = yarp.DVector(1)
        self.max3 = yarp.DVector(1)
        self.mentalControlLimits.getLimits(0, self.min3, self.max3)

        #self.lbound=-15
        #self.ubound=100

        '''
        #Set initial position:
        mentalPositionControl = self.mentalDevice.viewIPositionControl()

        dEncRaw = np.empty(6,float)

        dEncRaw[0] = 0
        dEncRaw[1] = 0
        dEncRaw[3] = 0

        mentalPositionControl.positionMove(0, dEncRaw[0])
        mentalPositionControl.positionMove(1, dEncRaw[1])
        mentalPositionControl.positionMove(3, dEncRaw[3])
        '''

        #time.sleep(self.yarpDelay)  # pause 5.5 seconds

        ################ YARP CONNECT TO PAINT #############################
        remotePaint = "/drl/openraveYarpPaintSquares/rpc:s"
        localPaint = "/cgda/openraveYarpPaintSquares/rpc:c"

        self.rpcClient=yarp.RpcClient()
        self.rpcClient.open(localPaint) #Connect to local paint

        while True: ##do-while
            yarp.Network.connect(localPaint,remotePaint)
            print("Wait to connect to paint server...\n")
            time.sleep(self.yarpDelay)
            if self.rpcClient.getOutputCount() != 0:
                break
        print ("Paint server available. \n")

        #Read state
        self.cmd= yarp.Bottle()
        self.res= yarp.Bottle()

        '''
        # Reset paint
        self.cmd.clear()
        self.res.clear()
        self.cmd.addString("reset")
        print(self.cmd)
        self.rpcClient.write(self.cmd,self.res)
        '''

        ############### GENERAL ############################################

        #
        Serializable.quick_init(self, locals())

        self.start_state = [0,0,0]
        self.state = None
        self.domain_fig = None

    ################### RESET #############################

    def reset(self):
        self.state = copy.deepcopy(self.start_state) #rmbr

        #Set initial position:
        dEncRaw = np.empty(6,float)

        dEncRaw[0] = 0
        dEncRaw[1] = 0
        dEncRaw[3] = 0

        self.mentalPositionControl.positionMove(0, dEncRaw[0])
        self.mentalPositionControl.positionMove(1, dEncRaw[1])
        self.mentalPositionControl.positionMove(3, dEncRaw[3])

        print(self.state)
        time.sleep(self.yarpDelay)  # pause

        # Reset paint
        self.cmd.clear()
        self.res.clear()
        self.cmd.addString("reset")
        print(self.cmd)
        self.rpcClient.write(self.cmd,self.res)

        #Reset num_steps
        self.num_step = 0

        #Set percentage of painted wall as zero
        self.percentage = 0


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
        """
        action map:
        0: left
        1: down
        2: right
        3: up
        4: climb_up
        5: climb_down
        :param action: should be a one-hot vector encoding the action
        :return:
        """

        '''
        ################### GET ACTUAL STATE #############################
        self.cmd.clear()
        self.res.clear()
        self.cmd.addString("get")
        self.cmd.addString("encs")
        self.rpcClientCart.write(self.cmd, self.res) #Obtain the cartesian position.

        #print("La respuesta de posición es: ", self.res)
        #print("Got position now: ", self.res.toString())
        #printf("Got: %s\n", res.toString().c_str());
        '''

        '''
        #GET ENCODERS
        enc = self.mentalDevice.viewIEncoders()  # make an encoder controller object we call 'enc'
        axes = enc.getAxes()
        v = yarp.DVector(axes)  # create a YARP vector of doubles the size of the number of elements read by enc, call it 'v'
        enc.getEncoders(v) # read the encoder values and put them into 'v'

        self.state=[]
        self.state.append(v[0])
        self.state.append(v[1])
        self.state.append(v[3])

        #for i in range(self.res.size()):
        #    state_now.append(self.res.get(i).asDouble())
        print("The state now is: ",self.state)
        '''

        possible_next_states = self.get_possible_next_states(self.state, action)

        #x = self.state // self.n_col
        #y = self.state % self.n_col
        #coords = np.array([x, y])
        
        #print(coords)
        #now=self.desc
        #now[coords[0], coords[1]]='X'
        #print(now)

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
        self.state[0] = next_state[0]
        self.state[1] = next_state[1]
        self.state[2] = next_state[2]

        #dEncRaw[0] = 45
        #dEncRaw[1] = 0
        #dEncRaw[3] = 0

        self.mentalPositionControl.positionMove(0, dEncRaw[0])
        self.mentalPositionControl.positionMove(1, dEncRaw[1])
        self.mentalPositionControl.positionMove(3, dEncRaw[3])

        #Just some sleep for debug
        #print("Moving the robot")
        time.sleep(self.yarpDelay)  # pause

        ################ GET POSITION #################################

        '''
        self.cmd.clear()
        self.res.clear()
        self.cmd.addString("get encs")
        self.rpcClientCart.write(self.cmd, self.res)

        #print("La posición después de moverme es: ", self.res)
        print("Got after movement : ", self.res.toString())

        print("--------------------------------------------------------------\n")
        '''


        #print(self.n_col)
        #print(self.n_row)
        #print(self.levels)
        #print("the next z is", next_z)
        #print("the next x is", next_x)
        #print("the next y is", next_y)

        ################ YARP GET % PAINT #################################

        # self.cmd= "get"
        self.cmd.clear()
        self.res.clear()
        self.cmd.addString("get")
        self.rpcClient.write(self.cmd, self.res)

        #print("The size of print after get is: ", self.res.size())

        new_percentage=0
        for i in range(self.res.size()):
            new_percentage= new_percentage+self.res.get(i).asInt()
            # print(i)

        new_percentage=(new_percentage*100/self.res.size())

        #If a paint additional wall reward=1
        if new_percentage > self.percentage:
            #print("******************************************************************************************** painted :)")
            reward=1
            #quit()
        elif new_percentage < self.percentage:
            reward=-1
        else:
            print("nothing painted :(")
            reward=0-self.num_step*self.action_penalty

        '''
        self.num_step=self.num_step+1

        self.percentage = 0

        reward=percentage-self.num_step*self.action_penalty
        '''
        self.percentage=new_percentage

        if self.percentage==100:
            done=True
        else:
            done=False

        print ("El reward es: ", reward)

        return Step(observation=self.state, reward=reward, done=done)

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
