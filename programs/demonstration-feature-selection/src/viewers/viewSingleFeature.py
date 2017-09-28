# -*- coding: utf-8 -*-
"""
Author: Santiago Morante
Robotics Lab. Universidad Carlos III de Madrid
"""
################### GENERAL ##############################
import glob
import libtools

##########################   DTW   ########################
import matplotlib.pyplot as plt


NORM_FOLDER = "PHYSICAL" # NONE MINMAX STANDARDIZED WHOLE_EXPERIMENT PHYSICAL
COLLAPSE_FOLDER = "cols" # rows cols
EXPERIMENT = "experiment-1"
PATH = "../../datasets/" + EXPERIMENT +"/selected-demonstrations/" + NORM_FOLDER + "/" + COLLAPSE_FOLDER  +"/*.csv"

NORMALIZATION = "PHYSICAL" # NONE MINMAX STANDARDIZED WHOLE_EXPERIMENT PHYSICAL
  
#################################        
def main():
    
    demons=[]
    demoNames = sorted(glob.glob(PATH))
    demons = libtools.normalize(NORMALIZATION,demoNames)
   
    ##########################   PLOT ##################################
        
    for f in range(demons[0].shape[1]): # number of columns minus 1 for timestamp
        fig = plt.figure()
        plt.title(str(f+1))
        for i in range(len(demoNames)):
            plt.plot(demons[i][:,f])

    ####################################################################

if __name__ == "__main__":
    main()