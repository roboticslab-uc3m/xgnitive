# -*- coding: utf-8 -*-
"""
Author: Raul Fernandez-Fernandez
Robotics Lab. Universidad Carlos III de Madrid
"""

################### GENERAL ##############################
import glob
import numpy as np

########## TO READ FILES ###################
EXPERIMENT_FOLDER = "/icra2018"
ACTION_FOLDER= "/paint"
FEATURE_FOLDER= "/jr3" #for icra2018 there are 4 possible folders: "/jr3", "/paintSquaresOnScreen", "/teo/leftArm" and "/teo/rightArm".

PATH ="../datasets/raw"+ EXPERIMENT_FOLDER + ACTION_FOLDER + FEATURE_FOLDER + "/*.log"
SAVE_PATH="../datasets/raw"+ EXPERIMENT_FOLDER + ACTION_FOLDER + FEATURE_FOLDER
print (PATH)

########## TO USE NOW ###################
frequency=10 #The frequency at which the selector pick a row. The number of raws of the file is divided by this frequency.

#########################################

def main():

    demoNames = sorted(glob.glob(PATH))
    if not demoNames:
        print "[ERROR] No files to read in PATH!"
        return False
    
    j=0 
    for elem in demoNames:
        selector=[]
        j=j+1
        tmp=np.loadtxt(elem)
        for i in range(0,tmp.shape[0],frequency): #Number of rows
            selector.append(tmp[i])
        np.savetxt(SAVE_PATH+"/reduced-data-"+str(j)+".csv",selector, fmt='%f')
        #np.savetxt("saved.csv",selector, fmt='%d')

if __name__ == "__main__":
        main()
