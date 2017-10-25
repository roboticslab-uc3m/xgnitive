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
'''
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
'''

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

    def __init__(self, desc='4x4'):

        self.yarpDelay=0.5

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

        ################ YARP CONNECT TO ENCODERS ############################

        self.rpcClientCart = yarp.RpcClient()
        self.rpcClientCart.open("/encs:c")
        while True:
            yarp.Network.connect("/encs:c", "/teoSim/rightArm/rpc:i")
            print("Wait to connect to rightArm server...\n")
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

        #print("next state before move is", next_state)

        '''
        next_z = next_state // (self.n_col * self.n_row)
        next_x = (next_state - next_z*(self.n_col * self.n_row)) // self.n_col #Note: this is not a comment :D
        next_y = (next_state - next_z*(self.n_col * self.n_row)) % self.n_col
        '''

        #Declare device to position control
        mentalPositionControl = self.mentalDevice.viewIPositionControl()

        #Try to move the robot. We may need cartesians.

        #PosX= coordX*sizeXcelda
        #PosY= coordY*sizeYcelda
        #PosZ= coordZ*sizeZcelda

        #print("the next state is ",next_state)
        print("Me voy a mover a:", next_state[0],next_state[1],next_state[2])
        dEncRaw = np.empty(6,float)
        '''
        dEncRaw[0] = next_state[0]
        dEncRaw[1] = -next_state[1]
        dEncRaw[3] = next_state[2]
        '''
        dEncRaw[0] = 30
        dEncRaw[1] = 0
        dEncRaw[3] = 0

        mentalPositionControl.positionMove(0, dEncRaw[0])
        mentalPositionControl.positionMove(1, dEncRaw[1])
        mentalPositionControl.positionMove(3, dEncRaw[3])

        #Just some sleep for debug
        #print("Moving the robot")
        time.sleep(self.yarpDelay)  # pause 5.5 seconds

        ################ GET POSITION #################################

        self.cmd.clear()
        self.res.clear()
        self.cmd.addString("get encs")
        self.rpcClientCart.write(self.cmd, self.res)

        #print("La posición después de moverme es: ", self.res)
        print("Got after movement : ", self.res.toString())

        '''
        # Now:
        state_now=[]
        for i in range(1, 4): #take only the 3 first
            state_now.append(self.res.get(i).asDouble())
        print(state_now)
        '''
        print("--------------------------------------------------------------\n")


        #print(self.n_col)
        #print(self.n_row)
        #print(self.levels)
        #print("the next z is", next_z)
        #print("the next x is", next_x)
        #print("the next y is", next_y)

        '''
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
        '''

        ################ YARP GET % PAINT #################################

        # self.cmd= "get"
        self.cmd.clear()
        self.res.clear()
        self.cmd.addString("get")
        self.rpcClient.write(self.cmd, self.res)

        #print("The size of print after get is: ", self.res.size())

        percentage=0
        for i in range(self.res.size()):
            percentage= percentage+self.res.get(i).asInt()
            # print(i)

        percentage=(percentage*100/self.res.size())

        reward=percentage

        if percentage==100:
            done=True
        else:
            done=False

        #print ("El reward es: ", reward)

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
        self.cmd.addString("get")
        self.cmd.addString("encs")
        self.rpcClientCart.write(self.cmd, self.res) #Obtain the cartesian position.

        #print("La respuesta de posición es: ", self.res)
        #print("Got position now: ", self.res.toString())
        #printf("Got: %s\n", res.toString().c_str());

        enc = self.mentalDevice.viewIEncoders()  # make an encoder controller object we call 'enc'
        axes = enc.getAxes()
        v = yarp.DVector(axes)  # create a YARP vector of doubles the size of the number of elements read by enc, call it 'v'
        enc.getEncoders(v) # read the encoder values and put them into 'v'
        print("the axes are: ",str(v[0]))

        # Now:
        response=self.res.toString()
        response = response.replace("(", "") #Delete parenthesis from the string
        response=response.split(" ") #split string
        print(response[2])
        print(response[3])
        print(response[5])


        state_now=[]
        state_now.append(response[2])
        state_now.append(response[3])
        state_now.append(response[5])

        #for i in range(self.res.size()):
        #    state_now.append(self.res.get(i).asDouble())
        print("The state now is: ",state_now)

        ################### GET FUTURE STATES #############################

        #Possible increments produced by the action:
        increments = np.array([[5, 0, 0], [-5, 0, 0], [0, 5, 0], [0, -5, 0], [0, 0, 5], [0, 0, -5]])

        next_state = np.clip(
            state_now + increments[action],
            [self.lbound, self.lbound, self.lbound], # Limits
            [self.ubound, self.ubound, self.ubound]  # Limits
        )

        print("the next state is ", next_state)

        '''
        #print(next_coords)
        next_state = next_coords[0] * (self.n_col + self.n_row) + next_coords[1] * self.n_col + next_coords[2] #Calculate next step
        #print(next_state)
        state_type = self.desc[z, x, y]
        next_state_type = self.desc[next_coords[0], next_coords[1], next_coords[2]]
        #print(next_state_type)
        if next_state_type == 'W' or state_type == 'H' or state_type == 'G':
            return [(state, 1.)]
        else:
        '''

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
