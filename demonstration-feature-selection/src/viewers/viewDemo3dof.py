# -*- coding: utf-8 -*-
"""
Author: Juan
Robotics Lab. Universidad Carlos III de Madrid
"""
################### GENERAL ##############################
import glob
import numpy as np

import numpy as np
import matplotlib.pyplot as plt

EXPERIMENT = "experiment-1"
PATH = "../../datasets/" + EXPERIMENT +"/raw/*.csv"  
OBJECT = 2
#################################        
def main():

    fig = plt.figure()
    ax = fig.gca(projection='3d')
    
    demons=[]
    demoNames = sorted(glob.glob(PATH))

    NUM_COLORS = len(demoNames)

    cm = plt.get_cmap('gist_rainbow')
    ax.set_color_cycle([cm(1.*i/NUM_COLORS) for i in range(NUM_COLORS)])
    for elem in demoNames:
        print elem
        tmp = np.loadtxt(elem)
        if OBJECT == 1:
            tmp_clean = tmp[:,1:4]
        if OBJECT == 2:
            tmp_clean = tmp[:,4:7]           
        else:
            print "[ERROR] Unknown object!"
        #print tmp_clean
        ax.plot(tmp_clean[:,0], tmp_clean[:,1], tmp_clean[:,2], label=elem.split('/')[-1], lw=3)
        plt.tick_params(axis='both', labelsize=25)
        ax.set_xlabel('X', fontsize=30)
        ax.set_ylabel('Y', fontsize=30)
        ax.set_zlabel('Z', fontsize=30)
        ax.legend(prop={'size':19})

    plt.show()

##############################################################################
##############################################################################

if __name__ == "__main__":
    main()
