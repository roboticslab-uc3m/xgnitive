# -*- coding: utf-8 -*-
"""
@author: smorante
"""

import numpy as np
from sklearn import preprocessing
from scipy import stats
import matplotlib.pyplot as plt


def sumAll(demo1, demo2, method):
#    print "Shapes demo1,demo2: ", np.array(demo1).shape, "  , ",  np.array(demo2).shape
    if np.array(demo1).shape[0] > 1 and \
       np.array(demo2).shape[0] > 1 and \
       np.array(demo1).shape[1] == np.array(demo2).shape[1]:
           
           
        if method=="rows":
  #              print "[INFO] Collapsing rows"
            x = np.array(demo1).sum(axis=0)  # zero collaps rows into single row
            y = np.array(demo2).sum(axis=0)
        elif method=="cols":
            
   #             print "[INFO] Collapsing cols"
            x = np.array(demo1).sum(axis=1)  # zero collaps columns into single column
            y = np.array(demo2).sum(axis=1)
        
        else:
            print "[ERROR] Unrecognized method for summing!"
            return False
        
    elif(np.array(demo1).shape[1] != np.array(demo2).shape[1]):
        print "[ERROR] NumberOfColumns(demo1) != NumberOfColumns(demo2) "
        return False

    else:
        print "[ERROR] Unknown problem when summing rows or cols"
        return False
            
        x = np.array(demo1).flatten()
        y = np.array(demo2).flatten()

    return x, y

def plot_zscores(ALPHA, zscores, xtitle, SAVE_PATH,norm):
    fig = plt.figure()
    plt.bar(np.arange(len(zscores)), zscores, alpha=0.4, color='b')

    plt.xlabel(xtitle, fontsize=30)
    plt.xticks( fontsize=20)
    plt.xlim(0,len(zscores))
    plt.ylabel(norm + ' normalization', fontsize=30)
    plt.yticks( fontsize=20)

    plt.axhline(y=ALPHA, xmin=0, xmax=1, hold=None, color='red', lw=4, linestyle='--')
    plt.legend()
    plt.tight_layout()
    #plt.show()
    plt.savefig(SAVE_PATH+ "/"+norm+".pdf")
    
    
def normalize(normalization, demoNames,FEATURE_RANGE):
    
    demons=[]
    print "[INFO] Normalization: ", normalization

    if normalization == "NONE":
        ## for demonstration normalization
            for elem in demoNames:
                tmp = np.loadtxt(elem)
                tmp_clean = tmp[:,FEATURE_RANGE]
                demons.append(tmp_clean)
            return demons
    elif normalization == "MINMAX":
        ## for demonstration normalization
            for elem in demoNames:
                tmp = np.loadtxt(elem)
                tmp_clean = tmp[:,FEATURE_RANGE]
                tmp_clean = minmaxscale(tmp_clean)
                demons.append(tmp_clean)
            return demons
               
    elif normalization == "STANDARDIZED":
        ## for demonstration normalization
            for elem in demoNames:
                tmp = np.loadtxt(elem)
                tmp_clean = tmp[:,FEATURE_RANGE]
                tmp_clean = standardize(tmp_clean)
                demons.append(tmp_clean)     
            return demons

    elif normalization == "PHYSICAL":
        ## for demonstration normalization
            for elem in demoNames:
                tmp = np.loadtxt(elem)
                tmp_clean = tmp[:,FEATURE_RANGE]
                tmp_clean = physical_limits(tmp_clean)
                demons.append(tmp_clean)  
            return demons
                
    elif normalization == "WHOLE-EXPERIMENT":
        ### for whole experiment normalization
            demons_temp =[]
            for elem in demoNames:
                tmp = np.loadtxt(elem)
                tmp_clean = tmp[:,FEATURE_RANGE]
                demons.append(tmp_clean)
                demons_temp.extend(tmp_clean.tolist())
            ranges = np.max(np.array(demons_temp), axis=0) 
            print "[INFO] Maximum values per feature: ", ranges     
            demons=maxscale_precomputed(demons, ranges)
            return demons
    else:
            print "[ERROR] Unrecognized normalization"
            return False


def standardize(X):
    return preprocessing.scale(X, axis=0)

def minmaxscale(X):
   min_max_scaler = preprocessing.MinMaxScaler()
   return min_max_scaler.fit_transform(X)

def maxscale_precomputed(X, ranges):
    X_scaled = []
    for elem in X:
        temp= elem/ranges
        X_scaled.append(temp)
    return X_scaled     

def physical_limits(X):
    ranges=np.zeros(([13,2])) # one per feature. minus one because timestamp
    ranges[0] = [500,3000] # x
    ranges[1] = [-2000,2000] # y
    ranges[2] = [-1000,1000] # z
    ranges[3] = [500,3000] # x
    ranges[4] = [-2000,2000] # y
    ranges[5] = [-1000,1000] # z
    ranges[6] = [-2500,2500] # x
    ranges[7] = [-4000,4000] # y
    ranges[8] = [-2000,2000] # z  
    ranges[9] = [0,2500] # x
    ranges[10] = [0,4000] # y
    ranges[11] = [0,2000] # z      
    ranges[12] = [0,5120] # euclidean      
    X_scaled=np.zeros((X.shape )) # one per feature. minus one because timestamp
    for i in range(X.shape[1]): # number of columns
        X_scaled[:,i] = (X[:,i] -  ranges[i,0] ) / ( ranges[i,1] -  ranges[i,0])
    return X_scaled   
    

def get_outlier(points, thresh=2):
    if len(points.shape) == 1:
        points = points[:,None]
    results = stats.zscore(points) 
    return results.flatten() > thresh, results
