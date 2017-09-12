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


####################### TO USE NOW #######################



##########################################################
def main():

    #Read Trajectory 1

    Traj1=np.loadtxt("Trajectory1.txt")

    #Read Trajectory 2

    #Traj2= np.loadtxt("Trajectory2.txt")
    Traj2= np.loadtxt("Trajectory2_BEST.txt")

    ############################### CHOOSE BEST RESULT #########################
    
    #TODO implement this to be automatic

    ############################### PLOT #######################################

    for i in range(4):  # features
        plt.figure()
        plt.plot(np.linspace(0,1,Traj1.shape[0]),Traj1[:][:,i])
        plt.plot(Traj2[:][:,0]*1/8,Traj2[:][:,i+1])
	plt.plot(Traj2[:][:,0]*1/8,Traj2[:][:,i+1],'ro')


    plt.show()
    #plt.savefig('foo.png')




if __name__ == "__main__":
    main()
