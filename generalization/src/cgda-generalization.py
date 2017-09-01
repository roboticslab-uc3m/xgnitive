# -*- coding: utf-8 -*-
"""
Author: Raul Fernandez-Fernandez (rauferna@ing.uc3m.es)
Robotics Lab. Universidad Carlos III de Madrid
"""

################### GENERAL ##############################
import glob
import numpy as np

######################## TO READ FILES ###################
EXPERIMENT_FOLDER = "/icra2018"
ACTION_FOLDER= "/paint"
FEATURE_FOLDER= "/jr3" #for icra2018 there are 4 possible folders: "jr3", "paintSquaresOnScreen", "/teo/leftArm" and "/teo/rightArm".

PATH ="../../demonstration-feature-selection/datasets/raw"+ EXPERIMENT_FOLDER + ACTION_FOLDER + FEATURE_FOLDER +"/*.csv"
#print (PATH)
SAVE_PATH="../results"+ EXPERIMENT_FOLDER + ACTION_FOLDER + FEATURE_FOLDER

######################## CONFIG PARAMS ###################

TIME_COLUMN=1 #Specify the column where the timestamp is saved in the dataset.

FEATURES=[2,3,4] #Specify the columns (feature) used for generalization.


##########################################################

def main():

    ###############################READ DATASETS#######################################
    demoNames = sorted(glob.glob(PATH)) #Access to the path for the .csv files
    if not demoNames:
        print "[ERROR] No files to read in PATH!"
        return False
    print demoNames

    demons=[]
    timeDiff=[]

    for elem in demoNames:
        tmp=np.loadtxt(elem);
        tmp_clean=tmp[:,FEATURES]
        timeDiff.append(np.ceil(tmp[-1][TIME_COLUMN] - tmp[0][TIME_COLUMN]))
        demons.append(tmp_clean)

    timeIntervals=int(np.mean(timeDiff)) #Get the average duration of the demonstration

    print timeIntervals

    #Time rescale
    r = float(tmp[-1][TIME_COLUMN] - tmp[0][TIME_COLUMN])
    tmp[:,1] = map(lambda x: (x - tmp[0][TIME_COLUMN]) / r, tmp[:,TIME_COLUMN])

    print tmp








if __name__ == "__main__":
    main()
