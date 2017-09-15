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

##########################################################
def main():

    #Read Trajectory 1

    Traj1=np.loadtxt("Trajectory-generalized.txt")

    #Read Trajectory 2

    #Traj2= np.loadtxt("Trajectory2.txt")
    #Traj2= np.loadtxt("Trajectory_OET_BESTS.txt")

    Traj3= np.loadtxt("trajectory-IET.txt")

    #Traj4= np.loadtxt("Trajectory_FTE.txt")

    ############################### CHOOSE BEST RESULT #########################
    
    #TODO implement this to be automatic

    ############################### PLOT #######################################

    for i in range(4):  # features
        plt.figure()
        plt.plot(np.linspace(0,1,Traj1.shape[0]),Traj1[:][:,i])
        #plt.plot(Traj2[:][:,0]*1/8,Traj2[:][:,i+1])
	#plt.plot(Traj2[:][:,0]*1/8,Traj2[:][:,i+1],'ro')
	plt.plot(np.linspace(0,1,9),Traj3[:][:,i])
        #plt.plot(np.linspace(0,1,9),Traj4[:][:,i])
   


    plt.show()
    #plt.savefig('foo.png')




if __name__ == "__main__":
    main()
