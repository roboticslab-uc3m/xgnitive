# -*- coding: utf-8 -*-
"""
Author: Raul Fernandez-Fernandez (rauferna@ing.uc3m.es)
Robotics Lab. Universidad Carlos III de Madrid
"""

################### GENERAL ##############################
import glob
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

font = {'family' : 'normal',
        'size'   : 18}

plt.rc('font', **font)

##########################################################
def main():

    #Read Trajectory 1

    Traj1=np.loadtxt("Trajectory-generalized.txt")

    #Read Trajectory 2

    #Traj2= np.loadtxt("Trajectory2.txt")
    Traj2= np.loadtxt("iron/Trajectory-OET.txt") #First line of OET is to specify the Time interval, not always T.

    Traj3= np.loadtxt("iron/Trajectory-IET.txt")

    Traj4= np.loadtxt("iron/Trajectory-FTE.txt")

    ############################### CHOOSE BEST RESULT #########################
    
    #TODO implement this to be automatic

    ############################### PLOT #######################################

    for i in range(4):  # features
        plt.figure()
	plt.xlabel('Force (N)')
	plt.ylabel('Time Interval')
        plt.plot(np.linspace(0,9,Traj1.shape[0]),Traj1[:][:,i])
        #plt.plot(Traj2[:][:,0]*1/8,Traj2[:][:,i+1])
	plt.plot(Traj2[:][:,0],Traj2[:][:,i+1])
	plt.plot(np.linspace(0,9,9),Traj3[:][:,i])
        plt.plot(np.linspace(0,9,9),Traj4[:][:,i])
   


    plt.show()
    #plt.savefig('foo.png')




if __name__ == "__main__":
    main()
