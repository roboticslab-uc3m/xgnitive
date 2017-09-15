# -*- coding: utf-8 -*-
"""
Author: Raul Fernandez-Fernandez (rauferna@ing.uc3m.es)
Robotics Lab. Universidad Carlos III de Madrid
"""

################### GENERAL ##############################
import numpy as np

################### TO USE NOW ###########################

features1=[] #column features to use from trajectory1

features1=[] #column features to use from trajectory1

t1="Trajectory1.txt" #Trajectory 1 file name

t2="Trajectory2.txt" #Trajectory 2 file name

##########################################################
def main():
    
    #Read Trajectory 1
    trajectory1=np.loadtxt(t1)
    t1_clean=trajectory1(:,features1)

    #Read Trajectory 2
    trajectory2= np.loadtxt(t2)
    t2_clean=trajectory2(:,features2)

    #Calculate mean and variance from trajectories, indeed we just need 1
	
    




if __name__ == "__main__":
    main()

