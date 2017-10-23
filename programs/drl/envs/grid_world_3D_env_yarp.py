################### GENERAL ##############################
import numpy as np
import time

################### RLLAB ################################
from .base import Env
from rllab.spaces import Discrete
from rllab.envs.base import Step
from rllab.core.serializable import Serializable

################### YARP ##############################
import yarp

################### TO USE NOW ############################

MAPS = {
    "chain": [
        "GFFFFFFFFFFFFFSFFFFFFFFFFFFFG"
    ],
    "4x4_safe": [
        "SFFF",
        "FWFW",
        "FFFW",
        "WFFG"
    ],
    "4x4": [
     [
        "SFFF",
        "FFFF",
        "FFFF",
        "FFFF"
    ],
    [
        "FFFF",
        "FFFF",
        "FFFF",
        "FFFF"
    ]
    ],
    "8x8": [
        "FFFFSFFF",
        "FFFFFFFF",
        "FFFHFFFF",
        "FFFFFHFF",
        "FFFHFFFF",
        "FHHFFFHF",
        "FHFFHFHF",
        "FFFHFFFG"
    ],
}

################### ENVIRONMENT #############################

class GridWorld3DEnvYarp(Env, Serializable):
    """
    'S' : starting point
    'F' or '.': free space
    'W' or 'x': wall
    'H' or 'o': hole (terminates episode)
    'G' : goal
    """

    ################### INIT ###############################

    def __init__(self, desc='4x4'):

        self.yarpDelay=0.05

        ################ YARP ###############################################
        yarp.Network.init()

        if yarp.Network.checkNetwork() != True:  # let's see if there was actually a reachable YARP network
            print('[error] Please try running yarp server')  # tell the user to start one with 'yarp server' if there isn't any
            quit()
        
        ################ YARP CONNECT TO MOVE ###############################
        # This device will have three different options, device, remote local.
        mentalOptions= yarp.Property()
        mentalOptions.put("device", "remote_controlboard") #device
        remoteMental="/teoSim/rightArm"
        mentalOptions.put("remote", remoteMental) #remote
        localMental="/cgdaMental"
        mentalOptions.put("local", localMental) #local

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

        #mentalPositionControl = yarp.IPositionControl()
        #mentalDevice.view(mentalPositionControl)

        ################ YARP CONNECT TO PAINT #############################
        remotePaint = "/openraveYarpPaintSquares/rpc:s"
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

        # Reset paint
        #  8self.cmd= "reset"
        self.cmd.clear()
        self.res.clear()
        self.cmd.addString("reset")
        print(self.cmd)
        self.rpcClient.write(self.cmd,self.res)

        ################ YARP CARTESIAN CONTROL ############################

        self.rpcClientCart = yarp.RpcClient()
        self.rpcClientCart.open("/cart:c")
        while True:
            yarp.Network.connect("/cart:c", "/CartesianControl/rpc_transform:s")
            print("Wait to connect to cartesian server...\n")
            #time.sleep(self.yarpDelay)
            if self.rpcClientCart.getOutputCount() != 0:
                break

        ############### GENERAL ############################################

        # Do something I dont know what
        Serializable.quick_init(self, locals())


        '''
        #print("desc before isinstance",desc)
        if isinstance(desc, str):
            desc = MAPS[desc]

        #print("desc before nparray \n",desc)
        desc[0] = list(map(list, desc[0]))
        #print(desc[0])
        desc[1] = list(map(list, desc[1]))
        #print(desc[1])
        desc= np.array(list(desc))
        #print("desc after nparray \n",desc)
        desc[desc == '.'] = 'F'
        desc[desc == 'o'] = 'H'
        desc[desc == 'x'] = 'W'
        self.desc = desc
        self.levels, self.n_row, self.n_col = desc.shape[:]
        #print("desc before search start \n", desc)
        (start_z,), (start_x,), (start_y,) = np.nonzero(desc == 'S')
        print('x', start_x)
        print('y', start_y)
        print('z', start_z)   
        '''

        self.lbound=-15
        self.ubound=100

        self.start_state = [0,0,0]
        self.state = None
        self.domain_fig = None

    ################### RESET #############################

    def reset(self):
        self.state = self.start_state
        return self.state

    ################### ACTION DEFINITION #################

    @staticmethod
    def action_from_direction(d):
        """
        Return the action corresponding to the given direction. This is a helper method for debugging and testing
        purposes.
        :return: the action index corresponding to the given direction
        """
        return dict(
            left=0,
            down=1,
            right=2,
            up=3,
            climb_up=4,
            climb_down=5
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

        possible_next_states = self.get_possible_next_states(self.state, action)

        #x = self.state // self.n_col
        #y = self.state % self.n_col
        #coords = np.array([x, y])
        
        #print(coords)
        #now=self.desc
        #now[coords[0], coords[1]]='X'
        #print(now)

        probs = [x[1] for x in possible_next_states]
        next_state_idx = np.random.choice(len(probs), p=probs)
        next_state = possible_next_states[next_state_idx][0]

        print("next state is", next_state)

        next_z = next_state // (self.n_col * self.n_row)
        next_x = (next_state - next_z*(self.n_col * self.n_row)) // self.n_col #Note: this is not a comment :D
        next_y = (next_state - next_z*(self.n_col * self.n_row)) % self.n_col

        #Declare device to position control
        mentalPositionControl = self.mentalDevice.viewIPositionControl()

        #Try to move the robot. We may need cartesians.

        #PosX= coordX*sizeXcelda
        #PosY= coordY*sizeYcelda
        #PosZ= coordZ*sizeZcelda

        dEncRaw = np.empty(6,float)
        dEncRaw[0] = -2.4
        dEncRaw[1] = -45
        dEncRaw[3] = -37

        mentalPositionControl.positionMove(0, dEncRaw[0])
        mentalPositionControl.positionMove(1, dEncRaw[1])
        mentalPositionControl.positionMove(3, dEncRaw[3])

        #Just some sleep for debug
        print("Moving the robot")
        time.sleep(self.yarpDelay)  # pause 5.5 seconds

        #print(self.n_col)
        #print(self.n_row)
        #print(self.levels)
        #print("the next z is", next_z)
        #print("the next x is", next_x)
        #print("the next y is", next_y)

        next_state_type = self.desc[next_z, next_x, next_y]
        
        #print(next_state_type)
        #print(self.desc)

        # Here we fix what each position does.
        if next_state_type == 'H':
            done = True
            reward = 0
        elif next_state_type in ['F', 'S']:
            done = False
            reward = 0
        elif next_state_type == 'G':
            done = True
            reward = 1
        else:
            raise NotImplementedError
        self.state = next_state

        print("The size is: ", self.res.size())

        for i in range(self.res.size()):
            print(self.res.get(i).asInt())
            print(i)

        ################ YARP GET % PAINT #################################

        # self.cmd= "get"
        self.cmd.clear()
        self.res.clear()
        self.cmd.addString("get")
        self.rpcClient.write(self.cmd, self.res)

        print("The size of print after get is: ", self.res.size())

        percentage=0
        for i in range(self.res.size()):
            percentage= percentage+self.res.get(i).asInt()
            # print(i)

        percentage=(percentage*100/self.res.size())

        reward=percentage

        return Step(observation=self.state, reward=reward, done=done)

    ################### NEXT STATES #############################

    def get_possible_next_states(self, state, action):

        """
        Given the state and action, return a list of possible next states and their probabilities. Only next states
        with nonzero probabilities will be returned
        :param state: start state
        :param action: action
        :return: a list of pairs (s', p(s'|s,a))
        """

        # assert self.observation_space.contains(state)
        # assert self.action_space.contains(action)

        '''
        # Obtain the z,x,y of the system by its state number. Later pos_z=coord_z*(increment z celda)
        z = self.state // (self.n_col * self.n_row)
        x = (self.state - z*(self.n_col * self.n_row)) // self.n_col #Note: this is not a comment :D
        y = (self.state - z*(self.n_col * self.n_row)) % self.n_col
        coords = np.array([z, x, y])

        #print('NEW STEP')
        #print(coords)

        #print(coords)
        ## Debug things
        self.desc[0] = list(map(list, self.desc[0]))
        #print(desc[0])
        self.desc[1] = list(map(list, self.desc[1]))
        #print(desc[1])
        now= np.array(list(self.desc))
        #now=np.array(list(map(list, self.desc)))

        #print(now)

        now[z, x, y]='X'
        print(now)
	
        ## Possible increments produced by the actions.
        #print(action)
        increments = np.array([[0, 0, -1], [0, 1, 0], [0, 0, 1], [0, -1, 0], [1, 0, 0], [-1, 0, 0]])
        next_coords = np.clip(
            coords + increments[action],
            [0, 0, 0],
            [self.levels -1, self.n_row - 1, self.n_col - 1] #Set the limits
        )
        '''

        ################### GET POSITION #############################

        self.cmd.clear()
        self.res.clear()
        self.cmd.addString("stat")
        self.rpcClientCart.write(self.cmd, self.res)

        print("La respuesta de posción es: ", self.res)
        #printf("Got: %s\n", res.toString().c_str());

        #yarp.Time.delay(DEFAULT_DELAY_S);
        self.cmd.addString("stat")

        # *cmd.addString("world");
        # cmd.addString("whereis");
        # cmd.addString("tcp");
        # cmd.addString("rightArm"); * /
        pRpcClientCart.write(cmd, res);
        printf("Got: %s\n", res.toString().c_str());
        #print(next_coords)
        next_state = next_coords[0] * (self.n_col + self.n_row) + next_coords[1] * self.n_col + next_coords[2] #Calculate next step
        #print(next_state)
        state_type = self.desc[z, x, y]
        next_state_type = self.desc[next_coords[0], next_coords[1], next_coords[2]]
        #print(next_state_type)
        if next_state_type == 'W' or state_type == 'H' or state_type == 'G':
            return [(state, 1.)]
        else:
            return [(next_state, 1.)]

    ################### ACTION ##################################

    @property
    def action_space(self):
        return Discrete(6)

    ################### OBSERVATION #############################

    @property
    def observation_space(self):
        return Discrete(self.n_row * self.n_col * self.levels)
