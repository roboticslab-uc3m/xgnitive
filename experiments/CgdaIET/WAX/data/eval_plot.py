#!/usr/bin/python
import matplotlib.pyplot as plt 

font = {'family' : 'normal',
        'size'   : 18}

plt.rc('font', **font)

SST=plt.plotfile('SST.txt', cols=(0, 4), delimiter=' ', 
                     names=('Time interval', 'X','Y','Z', 'Number of evaluations'), marker='s', label='SST')

PSO=plt.plotfile('PSO.txt', cols=(0, 4), delimiter=' ', newfig=False,
                     names=('Time interval', 'X','Y','Z', 'Number of evaluations'), marker='+', label='PSO')

AFFG=plt.plotfile('AFFGPSO-3.txt', cols=(0, 4), delimiter=' ', newfig=False,
                     names=('Time interval', 'X', 'Y','Z','Number of evaluations'), marker='o', label='AFFG')

PSOFI=plt.plotfile('FIPSO.txt', cols=(0, 4), delimiter=' ', newfig=False,
                     names=('Time interval', 'X','Y','Z', 'Number of evaluations'), marker='^', label='PSOFI')


plt.legend(loc=2)
# plt.title('Evaluations vs time interval')

#plt.plotfile('AFFG-Ev-paint.txt', 'PSO-Ev-paint.txt', 'PSOFI-Ev-paint.txt', 'SST-Ev-paint.txt', subplots=False, delimiter=' ', newfig=False, names=('Time interval', 'Wall Painted (%)', 'Number of evaluations'), marker='o')

plt.savefig('WAX-EvInt.pdf')
plt.show()
