# -*- coding: utf-8 -*-
"""
Author: Raul Fernandez-Fernandez (rauferna@ing.uc3m.es)
Robotics Lab. Universidad Carlos III de Madrid
"""

################### GENERAL ##############################
import glob
import numpy as np
import matplotlib.pyplot as plt

######################## TO READ FILES ###################
EXPERIMENT_FOLDER = "/icra2018"
ACTION_FOLDER= "/paint"
FEATURE_FOLDER= "/teo/leftArm" #for icra2018 there are 4 possible folders: "jr3", "paintSquaresOnScreen", "/teo/leftArm" and "/teo/rightArm".

PATH ="../../demonstration-feature-selection/datasets/raw"+ EXPERIMENT_FOLDER + ACTION_FOLDER + FEATURE_FOLDER +"/*.csv"
#print (PATH)
SAVE_PATH="../results"+ EXPERIMENT_FOLDER + ACTION_FOLDER + FEATURE_FOLDER

######################## CONFIG PARAMS ###################

T=5
TIME_COLUMN=1 #Specify the column where the timestamp is saved in the dataset.
FEATURES=[2,3,4] #Specify the columns (feature) used for generalization.
#FEATURES= slice(2,None)

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
        tmp_clean=tmp[:,FEATURES] #tmp_clean [time_step][feature index]
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

    #print demons[0][:,5] #demons[demons_index][time_step, feature_number]

    ############################### TIME INTERVAL ########################################

    timeIntervals=int(np.mean(timeDiff))/T #Get number of time intervals using period T

    timeIntervalsDuration=1.0/timeIntervals

    print timeIntervalsDuration

    #timeStep=demons[0][:,0].shape[0]/float(timeIntervals) #Time step
    #print temp_demons

    ###############################GET THE AVERAGED TRAJECTORY#######################################

    #PSEUDOCODE
    #for each time step i
    #   for each feature j
    #       numberofpoints=0
    #       for each demon k
    #           sum  k values between demon_time[timeIntervalsDuration*(i),timeIntervalsDuration*(i+1)]
    #           numberofpoints++
    #   generalized[i][j]=sum/numberofpoints

    generalizedTrajectory=[]

    #centers[time_interval][feature]

    #featureSum=[len()]

    centers=np.zeros((timeIntervals,demons[1].shape[1]));

    for i in range(timeIntervals): #time intervals
        npoints=0
        for j in range(len(demoNames)): #demons
            for k in range(demons[j].shape[0]):  # sample
                if (timeIntervalsDuration*(i)<=temp_demons[j][k]<timeIntervalsDuration*(i+1)):
                    for l in range(demons[j].shape[1]):  # features
                        centers[i][l]=centers[i][l]+demons[j][k,l]
                    npoints=npoints+1
        for j in range(demons[0].shape[1]):  # features
            centers[i][j]=centers[i][j]/npoints

    print centers

    ############################### RBF ########################################


    ############################### PLOT #######################################

    #print np.linspace(0,1,timeIntervals)
    #print centers[:][:,0]

    for i in range(demons[0].shape[1]):  # features
        print i
        plt.figure()
        plt.plot(np.linspace(0,1,timeIntervals),centers[:][:,i],'ro')

        for j in range(len(demoNames)): #demons
            plt.plot(temp_demons[j],demons[j][:,i])

    plt.show()
    #plt.savefig('foo.png')













    #for i in range(0,demons[0].shape[0],timeStep):
    #    generalizedTrajectory.append()

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
