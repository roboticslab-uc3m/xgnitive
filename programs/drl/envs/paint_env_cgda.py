#!/usr/bin/env python

'''
paint-env-cgda.py program.

Description: Custom environment to perform de ``paint'' action using rllab.

Authors: Raul Fernandez-Fernandez (rauferna@ing.uc3m.es)

Universidad Carlos III de Madrid
'''
################### GENERAL ##############################
import numpy as np

################### RLLAB ################################
from rllab.envs.base import Env
from rllab.envs.base import Step
from rllab.spaces import Box


##########################################################
class Env(object):
    def step(self, action):
        """
        Run one timestep of the environment's dynamics. When end of episode
        is reached, reset() should be called to reset the environment's internal state.
        Input
        -----
        action : an action provided by the environment
        Outputs
        -------
        (observation, reward, done, info)
        observation : agent's observation of the current environment
        reward [Float] : amount of reward due to the previous action
        done : a boolean, indicating whether the episode has ended
        info : a dictionary containing other diagnostic information from the previous action
        """

        ##Read from openrave the state of the environment
        self._state
        x, y = self._state

        #Calculate the reward using the state
        reward = - (x ** 2 + y ** 2) ** 0.5

        ##Has reached our goal if true done =true
        done = abs(x) < 0.01 and abs(y) < 0.01

        ##Update observation as the state observed by the system
        next_observation = np.copy(self._state)

        ##return values
        return Step(observation=next_observation, reward=reward, done=done)1

    def reset(self):
        """
        Resets the state of the environment, returning an initial observation.
        Outputs
        -------
        observation : the initial observation of the space. (Initial reward is assumed to be 0.)
        """
        raise NotImplementedError

    @property
    def action_space(self):
        """
        Returns a Space object
        """
        return Box(low=-np.inf, high=np.inf, shape=(3,)) #3D actions (3 joints movement)

    @property
    def observation_space(self):
        """
        Returns a Space object
        """
        return Box(low=-np.inf, high=np.inf, shape=(4,)) #4D (x,y,z,Nsquares)
