# -*- coding: utf-8 -*-
"""
@author: smorante
"""

import numpy as np
from sklearn.manifold import MDS
import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection

class Mds:

    def __init__(self):
       self.mds=None
       self.n_components = None
       self.n_init = None
       self.metric=None
       self.max_iter = None
       self.eps = None
       self.random_state = None
       self.dissimilarity = None 
       self.n_jobs = None
       self.similarities=None
       self.pos=None
       self.npos=None


    def create(self, n_components, metric, max_iter, eps, random_state, dissimilarity, 
               n_jobs, n_init):
        self.n_components = n_components
        self.metric=metric
        self.max_iter = max_iter
        self.eps = eps
        self.random_state = random_state
        self.dissimilarity = dissimilarity
        self.n_jobs = n_jobs
        self.n_init=n_init
        
        if self.metric == True:
            self.mds = MDS(n_components=self.n_components, max_iter=self.max_iter, 
                           eps=self.eps, random_state=self.random_state,
                       dissimilarity=self.dissimilarity, n_jobs=self.n_jobs, n_init=self.n_init)
                       
            print self.mds.get_params()
            return True

        elif self.metric == False:
            self.mds = MDS(n_components=self.n_components, metric=self.metric, 
                           max_iter=self.max_iter, eps=self.eps, dissimilarity=self.dissimilarity, 
                           random_state=self.random_state, n_jobs=self.n_jobs, n_init=self.n_init)
            return True
            
        else:
            print "[ERROR] metric in mds not recognized (accepts only True or False)!"
            return False        
                   

    def compute(self, similarities):
        self.similarities=similarities
        
        if self.metric == True:
            self.pos=self.mds.fit(self.similarities).embedding_
            return self.pos

        elif self.metric == False:
            self.npos=self.mds.fit_transform(self.similarities)
            return self.npos

        else:
            print "[ERROR] metric in mds not recognized (accepts only True or False)!"
            return False
        
    def plot(self):
        fig = plt.figure()
        fig.canvas.set_window_title('MDS')
        ax = plt.axes([0., 0., 1., 1.])
        self.similarities = self.similarities.max() / self.similarities * 100
        self.similarities[np.isinf(self.similarities)] = 0

        
        if self.metric == True:
            points=self.pos

        elif self.metric == False:
            points=self.npos

        else:
            print "[ERROR] metric in mds not recognized (accepts only True or False)!"
            return False        
    
        plt.scatter(points[:, 0], points[:, 1], s=20, c='black')

        # Plot the edges
        start_idx, end_idx = np.where(points)
        
        #a sequence of (*line0*, *line1*, *line2*), where::
        #            linen = (x0, y0), (x1, y1), ... (xm, ym)
        segments = [[points[i, :], points[j, :]]
                    for i in range(len(points)) for j in range(len(points))]
        values = np.abs(self.similarities)
        lc = LineCollection(segments,
                            zorder=0, cmap=plt.cm.hot_r,
                            norm=plt.Normalize(0, values.max()))
        lc.set_array(self.similarities.flatten())
        lc.set_linewidths(0.5 * np.ones(len(segments)))
        ax.add_collection(lc)
        
        plt.show()        
              
    def fit_transform(self, X):
        return self.mds.fit_transform(X)
      


        