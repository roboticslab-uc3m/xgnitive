# -*- coding: utf-8 -*-
"""
Author: Santiago Morante
Robotics Lab. Universidad Carlos III de Madrid
"""
##########################   DTW   ####################################
import libmddtw
import matplotlib.pyplot as plt
from dtw import dtw
##########################   MDS   ####################################
import numpy as np
from sklearn.metrics import euclidean_distances
import libmds

##########################   DBSCAN   ####################################
import libdbscan
from sklearn.preprocessing import StandardScaler # to normalize


def normalize(X):
    return StandardScaler().fit_transform(X)

def main():
    
    NUMBER_OF_DEMONSTRATIONS=5
     ##########################################################################
     ##########################   DTW   ####################################
     ##########################################################################  

    dist=np.zeros((NUMBER_OF_DEMONSTRATIONS,NUMBER_OF_DEMONSTRATIONS))

    demons=[]
    
    # fill demonstrations
    for i in range(NUMBER_OF_DEMONSTRATIONS):
        demons.append(np.matrix([ np.sin(np.arange(15+i)+i)  , np.sin(np.arange(15+i)+i)]))

    
    # fill distance matrix    
    for i in range(NUMBER_OF_DEMONSTRATIONS):
        for j in range(NUMBER_OF_DEMONSTRATIONS):

            mddtw = libmddtw.Mddtw()
            x,y = mddtw.collapseRows(demons[i],demons[j])
            
            #fig = plt.figure()
            #plt.plot(x)
            #plt.plot(y)
            singleDist, singleCost, singlePath = mddtw.compute(demons[i],demons[j])
            dist[i][j]=singleDist
#            print 'Minimum distance found:', singleDist
            #fig = plt.figure()
        
        #    plt.imshow(cost.T, origin='lower', cmap=plt.cm.gray, interpolation='nearest')
        #    plt.plot(path[0], path[1], 'w')
        #    plt.xlim((-0.5, cost.shape[0]-0.5))
        #    plt.ylim((-0.5, cost.shape[1]-0.5))
            
#        print "dist", dist
    ###########################################################################
    ###########################   MDS   ####################################
    ###########################################################################

    md = libmds.Mds()
    md.create(n_components=1, metric=False, max_iter=3000, eps=1e-9, random_state=None,
                       dissimilarity="precomputed", n_jobs=1)

    points = md.compute(dist)
    print "points", points.flatten()
  #  md.plot()
    
    
    
    
     ##########################################################################
     ##########################   norm   ####################################
     ##########################################################################
    from scipy.stats import norm
    from numpy import linspace
    from pylab import plot,show,hist,figure,title
    
    param = norm.fit(points.flatten()) # distribution fitting
    x = linspace(np.min(points),np.max(points),NUMBER_OF_DEMONSTRATIONS)
    
    pdf_fitted = norm.pdf(x, loc=param[0],scale=param[1])

    
    fig = plt.figure()
    title('Normal distribution')    
    plot(x,pdf_fitted,'r-')
    hist(points.flatten(),normed=1,alpha=.3)
    show()


    for elem in points:

        if elem <= np.mean(points):        
            print "probability of point ", str(elem), ": ", norm.cdf(elem, loc=param[0],scale=param[1])
            
        if elem > np.mean(points):        
            print "probability of point ", str(elem), ": ", 1-norm.cdf(elem, loc=param[0],scale=param[1])


##############################################################################
##############################################################################

if __name__ == "__main__":
    main()