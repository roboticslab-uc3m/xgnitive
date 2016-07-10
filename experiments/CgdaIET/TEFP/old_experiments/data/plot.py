#!/usr/bin/python
import matplotlib.pyplot as plt 

plt.plotfile('AFFG-Ev-Paint.txt', delimiter=' ', cols=(0, 1), 
                     names=('Time interval', 'Wall Painted (%)'), marker='o')

plt.plotfile('Generalized.txt', delimiter=' ', cols=(0, 1), newfig=False,
                     names=('Time interval', 'Wall Painted (%)'), marker='o')


plt.plotfile('PSO-Ev-paint.txt', delimiter=' ', cols=(0, 1), 
                     names=('Time interval', 'Wall Painted (%)'), marker='o')

plt.plotfile('Generalized.txt', delimiter=' ', cols=(0, 1), newfig=False,
                     names=('Time interval', 'Wall Painted (%)'), marker='o')



plt.plotfile('PSOFI-Ev-paint.txt', delimiter=' ', cols=(0, 1), 
                     names=('Time interval', 'Wall Painted (%)'), marker='o')

plt.plotfile('Generalized.txt', delimiter=' ', cols=(0, 1), newfig=False,
                     names=('Time interval', 'Wall Painted (%)'), marker='o')


plt.plotfile('SST-Ev-paint.txt', delimiter=' ', cols=(0, 1), 
                     names=('Time interval', 'Wall Painted (%)'), marker='o')

plt.plotfile('Generalized.txt', delimiter=' ', cols=(0, 1), newfig=False,
                     names=('Time interval', 'Wall Painted (%)'), marker='o')

plt.show()
