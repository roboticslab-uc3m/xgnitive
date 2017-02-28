#!/usr/bin/python
import matplotlib.pyplot as plt 

font = {'family' : 'normal',
        'size'   : 18}

plt.rc('font', **font)

SST=plt.plotfile('SST-Ev-paint.txt', cols=(0, 2), delimiter=' ', 
                     names=('Time interval', 'Wall Painted (%)', 'Number of evaluations'), marker='s', label='SST')

PSO=plt.plotfile('PSO-Ev-paint.txt', cols=(0, 2), delimiter=' ', newfig=False,
                     names=('Time interval', 'Wall Painted (%)', 'Number of evaluations'), marker='+', label='PSO')

AFFG=plt.plotfile('AFFG-Ev-paint.txt', cols=(0, 2), delimiter=' ', newfig=False,
                     names=('Time interval', 'Wall Painted (%)', 'Number of evaluations'), marker='o', label='AFFG')

PSOFI=plt.plotfile('PSOFI-Ev-paint.txt', cols=(0, 2), delimiter=' ', newfig=False,
                     names=('Time interval', 'Wall Painted (%)', 'Number of evaluations'), marker='^', label='PSOFI')


plt.legend(loc=2)
# plt.title('Evaluations vs time interval')

#plt.plotfile('AFFG-Ev-paint.txt', 'PSO-Ev-paint.txt', 'PSOFI-Ev-paint.txt', 'SST-Ev-paint.txt', subplots=False, delimiter=' ', newfig=False, names=('Time interval', 'Wall Painted (%)', 'Number of evaluations'), marker='o')

plt.savefig('PAINT-EvInt.pdf')
plt.show()
