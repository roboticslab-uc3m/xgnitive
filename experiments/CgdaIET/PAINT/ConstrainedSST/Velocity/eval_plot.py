#!/usr/bin/python
import matplotlib.pyplot as plt 

font = {'family' : 'normal',
        'size'   : 18}

plt.rc('font', **font)


Low1=plt.plotfile('data-20.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval', 'Number of evaluations'), marker='s', label='Articular velocity (deg/iter) = 20')

Low2=plt.plotfile('data-60.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval', 'Number of evaluations'), marker='+', newfig=False, label='" " = 60')

Low3=plt.plotfile('data-80.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval', 'Number of evaluations'), marker='o', newfig=False, label='" " = 80')

Mid=plt.plotfile('data-100.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval', 'Number of evaluations'), marker='^', newfig=False, label='" " = 100')

High=plt.plotfile('data-120.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval', 'Number of evaluations'), marker='x', newfig=False, label='" " = 120')
 


leg=plt.legend(fancybox=True,loc=2)
leg.get_frame().set_alpha(0.5)
# plt.title('Evaluations vs time interval')

#plt.plotfile('AFFG-Ev-paint.txt', 'PSO-Ev-paint.txt', 'PSOFI-Ev-paint.txt', 'SST-Ev-paint.txt', subplots=False, delimiter=' ', newfig=False, names=('Time interval', 'Wall Painted (%)', 'Number of evaluations'), marker='o')

plt.savefig('PAINT-EvInt-Vel.pdf')
plt.show()
