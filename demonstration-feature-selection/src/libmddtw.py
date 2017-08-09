# -*- coding: utf-8 -*-
"""
@author: smorante
"""

import numpy as np
from dtw import dtw

class Mddtw:

    def create(self):
        return True
    
    def compute(self, demo1, demo2):
        dist, cost, path = dtw(demo1, demo2)
        return dist, cost, path

        