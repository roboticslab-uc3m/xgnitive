#!/usr/bin/python
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt 
import numpy as np

plt.plotfile('AFFGPSO-3.txt', delimiter=' ', cols=(1, 2), 
                     names=('i','X (mm)','Y (mm)'), marker='o')

plt.title('AFFG Experiment')

plt.plotfile('generalized.txt', delimiter=' ', cols=(1, 2), newfig=False,
                     names=('i','X (mm)','Y (mm)'), marker='o')

plt.savefig('AFFG-Paint.pdf')

plt.plotfile('PSO.txt', delimiter=' ', cols=(1, 2), 
                     names=('i','X (mm)','Y (mm)'), marker='o')

plt.title('PSO Experiment')

plt.plotfile('generalized.txt', delimiter=' ', cols=(1, 2), newfig=False,
                     names=('i','X (mm)','Y (mm)'), marker='o')

plt.savefig('PSO-Paint.pdf')


plt.plotfile('FIPSO.txt', delimiter=' ', cols=(1, 2), 
                     names=('i','X (mm)','Y (mm)'), marker='o')
plt.title('PSOFI Experiment')


plt.plotfile('generalized.txt', delimiter=' ', cols=(1, 2), newfig=False,
                     names=('i','X (mm)','Y (mm)'), marker='o')

plt.savefig('PSOFI-Paint.pdf')

plt.plotfile('SST.txt', delimiter=' ', cols=(1, 2), 
                     names=('i','X (mm)','Y (mm)'), marker='o')

plt.title('SST Experiment')

plt.plotfile('generalized.txt', delimiter=' ', cols=(1, 2), newfig=False,
                     names=('i','X (mm)','Y (mm)'), marker='o')

plt.savefig('SST-Paint.pdf')
plt.show()
