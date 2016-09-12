#!/usr/bin/python
import matplotlib.pyplot as plt 

font = {'family' : 'normal',
        'size'   : 18}

plt.rc('font', **font)

Adjustedp=plt.plotfile('data-Adjusted+.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval', 'Number of evaluations'), marker='s', label='Opening (m) = 0.05')

Adjusted=plt.plotfile('data-Adjusted.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval', 'Number of evaluations'), marker='o', newfig=False, label='" " = 0.1')

Intermediate=plt.plotfile('data-Intermediate.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval', 'Number of evaluations'), marker='^', newfig=False, label='" " = 0.2')

Large=plt.plotfile('data-Large.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval', 'Number of evaluations'), marker='x', newfig=False, label='" " = 0.3')
 


plt.legend(loc=2)
# plt.title('Evaluations vs time interval')

#plt.plotfile('AFFG-Ev-paint.txt', 'PSO-Ev-paint.txt', 'PSOFI-Ev-paint.txt', 'SST-Ev-paint.txt', subplots=False, delimiter=' ', newfig=False, names=('Time interval', 'Wall Painted (%)', 'Number of evaluations'), marker='o')

plt.savefig('WAX-EvInt.pdf')
plt.show()
