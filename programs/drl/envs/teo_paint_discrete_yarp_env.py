################### GENERAL ##############################
import numpy as np
import time

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

    ################### INIT ###############################

    def __init__(self):

        self.yarpDelay = 0.1
        self.action_inc = 5
        self.action_penalty = 0.1
        self.num_step = 0

        #Reward:
        self.percentage=0
        self.reward=0

        ################ YARP ###############################################
        yarp.Network.init()

        if yarp.Network.checkNetwork() != True:  # let's see if there was actually a reachable YARP network
            print(
            '[error] Please try running yarp server')  # tell the user to start one with 'yarp server' if there isn't any
            quit()

        ################ YARP CONNECT TO MOVE ###############################
        # This device will have three different options, device, remote local.
        mentalOptions = yarp.Property()
        mentalOptions.put("device", "remote_controlboard")  # device
        remoteMental = "/teoSim/rightArm"
        mentalOptions.put("remote", remoteMental)  # remote
        localMental = "/cgdaMental"
        mentalOptions.put("local", localMental)  # local

        # define Device
        # yarp.dev.Polydriver(mentalDevice)
        self.mentalDevice = yarp.PolyDriver()
        # open device
        self.mentalDevice.open(mentalOptions)
        if not self.mentalDevice.isValid():
            print("Mental robot device not available.\n")
            self.mentalDevice.close()
            yarp.Network.fini()
            return 1

        print("Mental robot device available.\n")

        # mentalPositionControl = yarp.IPositionControl()
        # mentalDevice.view(mentalPositionControl)

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

        time.sleep(self.yarpDelay)  # pause 5.5 seconds

        ################ YARP CONNECT TO PAINT #############################
        remotePaint = "/openraveYarpPaintSquares/rpc:s"
        localPaint = "/cgda/openraveYarpPaintSquares/rpc:c"

        self.rpcClient = yarp.RpcClient()
        self.rpcClient.open(localPaint)  # Connect to local paint

        while True:  ##do-while
            yarp.Network.connect(localPaint, remotePaint)
            print("Wait to connect to paint server...\n")
            time.sleep(self.yarpDelay)
            if self.rpcClient.getOutputCount() != 0:
                break
        print ("Paint server available. \n")

        # Read state
        self.cmd = yarp.Bottle()
        self.res = yarp.Bottle()
        '''
        # Reset paint
        self.cmd.clear()
        self.res.clear()
        self.cmd.addString("reset")
        print(self.cmd)
        self.rpcClient.write(self.cmd,self.res)
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

        ############### GENERAL ############################################

        #
        Serializable.quick_init(self, locals())

        self.lbound = -15
        self.ubound = 100

        self.start_state = [0, 0, 0]
        self.state = None
        self.domain_fig = None

    ################### RESET #############################

    def reset(self):
        self.state = self.start_state
        #Reward:
        self.reward = 0
        self.percentage = 0

        # Set initial position:
        mentalPositionControl = self.mentalDevice.viewIPositionControl()

        dEncRaw = np.empty(6, float)

        dEncRaw[0] = 0
        dEncRaw[1] = 0
        dEncRaw[3] = 0

        mentalPositionControl.positionMove(0, dEncRaw[0])
        mentalPositionControl.positionMove(1, dEncRaw[1])
        mentalPositionControl.positionMove(3, dEncRaw[3])

        time.sleep(self.yarpDelay)  # pause

        # Reset paint
        self.cmd.clear()
        self.res.clear()
        self.cmd.addString("reset")
        print(self.cmd)
        self.rpcClient.write(self.cmd, self.res)

        # Reset num_steps
        self.num_step = 0

        return self.state

    ################### ACTION DEFINITION #################

    @staticmethod
    def action_from_direction(d):
        """
        Return the action corresponding to the given direction. This is a helper method for debugging and testing
        purposes.
        :return: the action index corresponding to the given direction
        """
        return dict(  # The actions for the system are:
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

        ################### GET ACTUAL STATE #############################

        self.cmd.clear()
        self.res.clear()
        '''
        self.cmd.addString("get")
        self.cmd.addString("encs")
        self.rpcClientCart.write(self.cmd, self.res) #Obtain the cartesian position.
        #print("La respuesta de posición es: ", self.res)
        #print("Got position now: ", self.res.toString())
        #printf("Got: %s\n", res.toString().c_str());
        '''

        enc = self.mentalDevice.viewIEncoders()  # make an encoder controller object we call 'enc'
        axes = enc.getAxes()
        v = yarp.DVector(
            axes)  # create a YARP vector of doubles the size of the number of elements read by enc, call it 'v'
        enc.getEncoders(v)  # read the encoder values and put them into 'v'

        self.state = []
        self.state.append(v[0])
        self.state.append(v[1])
        self.state.append(v[3])

        # for i in range(self.res.size()):
        #    state_now.append(self.res.get(i).asDouble())
        print("The state now is: ", self.state)

        possible_next_states = self.get_possible_next_states(self.state, action)

        # x = self.state // self.n_col
        # y = self.state % self.n_col
        # coords = np.array([x, y])

        # print(coords)
        # now=self.desc
        # now[coords[0], coords[1]]='X'
        # print(now)

        probs = [x[1] for x in possible_next_states]
        next_state_idx = np.random.choice(len(probs), p=probs)
        next_state = possible_next_states[next_state_idx][0]

        # print("next state before move is", next_state)

        # Declare device to position control
        mentalPositionControl = self.mentalDevice.viewIPositionControl()

        # Try to move the robot. We may need cartesians.

        # print("the next state is ",next_state)
        print("Me voy a mover a:", next_state[0], next_state[1], next_state[2])
        dEncRaw = np.empty(6, float)

        dEncRaw[0] = next_state[0]
        dEncRaw[1] = next_state[1]
        dEncRaw[3] = next_state[2]

        # dEncRaw[0] = 30
        # dEncRaw[1] = 0
        # dEncRaw[3] = 0

        mentalPositionControl.positionMove(0, dEncRaw[0])
        mentalPositionControl.positionMove(1, dEncRaw[1])
        mentalPositionControl.positionMove(3, dEncRaw[3])

        # Just some sleep for debug
        # print("Moving the robot")
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

        # print(self.n_col)
        # print(self.n_row)
        # print(self.levels)
        # print("the next z is", next_z)
        # print("the next x is", next_x)
        # print("the next y is", next_y)

        ################ YARP GET % PAINT #################################

        ''''''
        # self.cmd= "get"
        self.cmd.clear()
        self.res.clear()
        self.cmd.addString("get")
        self.rpcClient.write(self.cmd, self.res)

        # print("The size of print after get is: ", self.res.size())

        '''
        #WORKING
        percentage = 0
        for i in range(self.res.size()):
            percentage = percentage + self.res.get(i).asInt()
            # print(i)

        percentage = (percentage * 100 / self.res.size())
        #self.num_step = self.num_step + 1

        reward = percentage#/10 #- self.num_step * self.action_penalty

        if percentage == 100:
            done = True
        else:
            done = False

        print ("El reward es: ", reward)

        '''
        new_percentage = 0
        for i in range(self.res.size()):
            new_percentage = new_percentage + self.res.get(i).asInt()
            # print(i)

        new_percentage = (new_percentage * 100 / self.res.size())

        self.reward = 0
        # If a paint additional wall reward=1
        if new_percentage > self.percentage:
            print("******************************************************************************************** painted :)")
            self.reward = +1000
            # quit()
        elif new_percentage < self.percentage:
            self.reward = -1000
        else:
            print("nothing painted :(")
            # self.reward= self.reward #-self.num_step*self.action_penalty

        #self.num_step = self.num_step + 1

        self.percentage = new_percentage
        print ("Percentage of painted wall is: ", self.percentage)

        if self.percentage == 100:
            done = True
        else:
            done = False

        print ("El reward es: ", self.reward)

        return Step(observation=self.state, reward=self.reward, done=done)

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

        # Possible increments produced by the action:
        increments = np.array(
            [[self.action_inc, 0, 0], [-self.action_inc, 0, 0], [0, self.action_inc, 0], [0, -self.action_inc, 0],
             [0, 0, self.action_inc], [0, 0, -self.action_inc]])

        next_state = np.clip(
            state + increments[action],
            [self.lbound, -self.ubound, self.lbound],  # Limits
            [self.ubound, -self.lbound, self.ubound]  # Limits
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
        return Box(low=-np.inf, high=np.inf, shape=(3,))  # The state is defined as a 3D continous space
        # return Discrete(self.n_row * self.n_col * self.levels)