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
FEATURES= slice(2,None)


##########################################################

def main():

    ###############################READ DATASETS#######################################
    demoNames = sorted(glob.glob(PATH)) #Access to the path for the .csv files
    if not demoNames:
        print "[ERROR] No files to read in PATH!"
        return False
    #print demoNames

    demons=[]
    temp_demons=[]
    timeDiff=[]


    for elem in demoNames:
        tmp=np.loadtxt(elem)
        #print tmp
        #select only choosen columns
        tmp_clean=tmp[:,FEATURES]
        #select time column
        temp0=tmp[:,TIME_COLUMN]
        # Get Number of time intervals of action
        timeDiff.append(np.ceil(temp0[-1] - temp0[0]))
        #Rescale
        r = float(temp0[-1] - temp0[0]) #range
        temp0=map(lambda x: (x - temp0[0]) / r,temp0[:]) #Actually rescale
        #Save values in matrix

        temp_demons.append(temp0)
        demons.append(tmp_clean)
        #TODO: 1) UNDERSTAND DEMOS; 2) CALCULATE AVERAGE OF N INTERVAL POINTS (timeIntervals) USING DEMONS AND TEMP_DEMONS; 3) INTERPOLATE WITH RBF.

    #print temp_demons[0][50]
    #print demons [0] [: 2]
    #print(demons)

    timeIntervals=int(np.mean(timeDiff)) #Get the average duration of the demonstration

    #print 'EL NUMERO DE INTERVALOS DE TIEMPO ES::::'
    #print timeIntervals
    #print 'EL NUEVO REESCALATION MATRIX ES::::::::::'
    #print realDataMatrix
    #print 'LAS FEATURES SON'
    #print demons

    #Time rescale
    #r = float(tmp[-1][TIME_COLUMN] - tmp[0][TIME_COLUMN])
    #tmp[:,1] = map(lambda x: (x - tmp[0][TIME_COLUMN]) / r, tmp[:,TIME_COLUMN])

    #print tmp

    #realDataMatrix = []
    #realDataMatrix.append(tmp.tolist())








if __name__ == "__main__":
    main()
