#!/usr/bin/python
import matplotlib.pyplot as plt 

AFFG=plt.plotfile('AFFG-Ev-paint.txt', cols=(0, 2), delimiter=' ',  
                     names=('Time interval', 'Wall Painted (%)', 'Number of evaluations'), marker='o', label='AFFG')


PSO=plt.plotfile('PSO-Ev-paint.txt', cols=(0, 2), delimiter=' ', newfig=False,
                     names=('Time interval', 'Wall Painted (%)', 'Number of evaluations'), marker='+', label='PSO')


PSOFI=plt.plotfile('PSOFI-Ev-paint.txt', cols=(0, 2), delimiter=' ', newfig=False,
                     names=('Time interval', 'Wall Painted (%)', 'Number of evaluations'), marker='^', label='PSOFI')


SST=plt.plotfile('SST-Ev-paint.txt', cols=(0, 2), delimiter=' ', newfig=False,
                     names=('Time interval', 'Wall Painted (%)', 'Number of evaluations'), marker='s', label='SST')

plt.legend(loc=2)
# plt.title('Evaluations vs time interval')

#plt.plotfile('AFFG-Ev-paint.txt', 'PSO-Ev-paint.txt', 'PSOFI-Ev-paint.txt', 'SST-Ev-paint.txt', subplots=False, delimiter=' ', newfig=False, names=('Time interval', 'Wall Painted (%)', 'Number of evaluations'), marker='o')

plt.savefig('EvavsInt.png')
plt.show()
