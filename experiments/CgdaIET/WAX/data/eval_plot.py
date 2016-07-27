#!/usr/bin/python
import matplotlib.pyplot as plt 

AFFG=plt.plotfile('AFFGPSO.txt', cols=(0, 4), delimiter=' ',  
                     names=('Time interval', 'X', 'Y','Z','Number of evaluations'), marker='o', label='AFFG')


PSO=plt.plotfile('PSO.txt', cols=(0, 4), delimiter=' ', newfig=False,
                     names=('Time interval', 'X','Y','Z', 'Number of evaluations'), marker='+', label='PSO')


PSOFI=plt.plotfile('FIPSO.txt', cols=(0, 4), delimiter=' ', newfig=False,
                     names=('Time interval', 'X','Y','Z', 'Number of evaluations'), marker='^', label='PSOFI')


SST=plt.plotfile('SST.txt', cols=(0, 4), delimiter=' ', newfig=False,
                     names=('Time interval', 'X','Y','Z', 'Number of evaluations'), marker='s', label='SST')

plt.legend(loc=2)
# plt.title('Evaluations vs time interval')

#plt.plotfile('AFFG-Ev-paint.txt', 'PSO-Ev-paint.txt', 'PSOFI-Ev-paint.txt', 'SST-Ev-paint.txt', subplots=False, delimiter=' ', newfig=False, names=('Time interval', 'Wall Painted (%)', 'Number of evaluations'), marker='o')

plt.savefig('EvavsInt.pdf')
plt.show()
