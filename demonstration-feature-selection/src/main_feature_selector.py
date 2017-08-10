# -*- coding: utf-8 -*-
"""
Author: Santiago Morante
Robotics Lab. Universidad Carlos III de Madrid
"""
################### GENERAL ##############################
import glob
import numpy as np
import libtools

##########################   DTW   ########################
import libmddtw
###########################################################

########## TO READ FILES ###################
NORM_FOLDER = "NONE" # NONE MINMAX STANDARDIZED WHOLE-EXPERIMENT PHYSICAL
SUM_FOLDER = "cols" # rows cols
ALPHA_FOLDER=0.5
DATABASE_FOLDER = "/icra2018"
EXPERIMENT_FOLDER= "/iron1"
FEATURE_FOLDER= "/jr3/ch2:o" #for icra2018 there are 4 possible folders: "jr3/ch2:o", "paintSquaresOnScreen/state:o", "teo/leftArm/CartesianControl/state:o" and "teo/rightArm/CartesianControl/state:o" .
PATH ="../datasets/raw"+ DATABASE_FOLDER + EXPERIMENT_FOLDER + FEATURE_FOLDER + "/data.log"
print (PATH)

############### TO USE NOW #####################
NORMALIZATION = "PHYSICAL" # NONE MINMAX STANDARDIZED WHOLE-EXPERIMENT PHYSICAL
ALPHA = 0.5
#################################        

def main():
    
    demoNames = sorted(glob.glob(PATH))
    if not demoNames:
        print "[ERROR] No files to read in PATH!"
        return False
    demons=[]
    demons = libtools.normalize(NORMALIZATION,demoNames)
     
    ##########################   DTW  + SUM #############################

    dist=np.zeros((demons[0].shape[1])) # one per feature. minus one because timestamp
         
    for f in range(demons[0].shape[1]): # number of columns minus 1 for timestamp
        for i in range(len(demoNames)):
            for j in range(len(demoNames)):
    
                mddtw = libmddtw.Mddtw()
                singleDist, singleCost, singlePath = mddtw.compute(demons[i][:,f], demons[j][:,f])
                dist[f]+=singleDist
                print 'Minimum distance found:', singleDist
#               fig = plt.figure()
#         #         plt.title(str(i)+","+str(j))
#               plt.imshow(singleCost.T, origin='lower', cmap=plt.cm.gray, interpolation='nearest')
#               plt.plot(singlePath[0], singlePath[1], 'w')
#               plt.xlim((-0.5, singleCost.shape[0]-0.5))
#               plt.xticks( fontsize=20)
#               plt.ylim((-0.5, singleCost.shape[1]-0.5))
#               plt.yticks( fontsize=20)  
            
    print "[INFO] Feature Dists List: ", dist
        
     ##########################   ZSCORE   #################################
    
    boolzscores, zscores = libtools.get_outlier(dist, thresh=ALPHA)
    print "[INFO] Detected outliers: ", boolzscores, zscores
    
    #####################  PLOT  ###########################################

    libtools.plot_zscores(ALPHA, zscores, "Features")

    ########################################################################

if __name__ == "__main__":
    main()