# -*- coding: utf-8 -*-
"""
Author: Raul Fernandez-Fernandez (rauferna@ing.uc3m.es)
Robotics Lab. Universidad Carlos III de Madrid
"""

################### GENERAL ##############################
import numpy as np

################### TO USE NOW ###########################

features1=[4] #column features to use from trajectory1

#features1=[] #column features to use from trajectory1

d1="data/trajectory-IET.txt" #Trajectory 1 file name

#d2="data/Data2.txt" #Trajectory 2 file name

##########################################################
def main():
    
    #Read Trajectory 1
    data1=np.loadtxt(d1)
    d1_clean=data1[:,features1]
    
	
    #Read Trajectory 2
    #trajectory2= np.loadtxt(t2)
    #t2_clean=trajectory2(:,features2)

    #Calculate mean and variance from trajectories, indeed we just need 1
    meanArray=[]
    varArray=[]
	
    for i in range(d1_clean.shape[1]):
        meanArray.append(np.mean(d1_clean[:][:,i]))
        varArray.append(np.var(d1_clean[:][:,i]))
    
    np.savetxt("results-mean.txt",meanArray,"%f")
    np.savetxt("results-var.txt",varArray, "%f")

if __name__ == "__main__":
    main()

