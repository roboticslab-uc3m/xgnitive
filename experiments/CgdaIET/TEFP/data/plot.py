#!/usr/bin/python
import matplotlib.pyplot as plt 

plt.plotfile('AFFG-Ev-paint.txt', delimiter=' ', cols=(0, 1), 
                     names=('Time interval', 'Wall Painted (%)'), marker='o')

plt.title('AFFG Experiment')

plt.plotfile('Generalized.txt', delimiter=' ', cols=(0, 1), newfig=False,
                     names=('Time interval', 'Wall Painted (%)'), marker='o')

plt.savefig('AFFG-Paint.png')

plt.plotfile('PSO-Ev-paint.txt', delimiter=' ', cols=(0, 1), 
                     names=('Time interval', 'Wall Painted (%)'), marker='o')

plt.title('PSO Experiment')

plt.plotfile('Generalized.txt', delimiter=' ', cols=(0, 1), newfig=False,
                     names=('Time interval', 'Wall Painted (%)'), marker='o')

plt.savefig('PSO-Paint.png')


plt.plotfile('PSOFI-Ev-paint.txt', delimiter=' ', cols=(0, 1), 
                     names=('Time interval', 'Wall Painted (%)'), marker='o')
plt.title('PSOFI Experiment')


plt.plotfile('Generalized.txt', delimiter=' ', cols=(0, 1), newfig=False,
                     names=('Time interval', 'Wall Painted (%)'), marker='o')

plt.savefig('PSOFI-Paint.png')

plt.plotfile('SST-Ev-paint.txt', delimiter=' ', cols=(0, 1), 
                     names=('Time interval', 'Wall Painted (%)'), marker='o')

plt.title('SST Experiment')

plt.plotfile('Generalized.txt', delimiter=' ', cols=(0, 1), newfig=False,
                     names=('Time interval', 'Wall Painted (%)'), marker='o')

plt.savefig('SST-Paint.png')
plt.show()
