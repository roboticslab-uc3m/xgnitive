#!/usr/bin/python
import matplotlib.pyplot as plt 
import matplotlib

font = {'family' : 'normal',
        'size'   : 16}

plt.rc('font', **font)

Adjustedpp=plt.plotfile('data-Adjusted++.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval (Adimensional)', 'Number of evaluations'), marker='s', label='0.01 m')

Adjustedp=plt.plotfile('data-Adjusted+.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval (Adimensional)', 'Number of evaluations'), marker='+', newfig=False, label='0.05 m')

Adjusted=plt.plotfile('data-Adjusted.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval (Adimensional)', 'Number of evaluations'), marker='o', newfig=False, label='0.1 m')

Intermediate=plt.plotfile('data-Intermediate.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval (Adimensional)', 'Number of evaluations'), marker='^', newfig=False, label='0.2 m')

Large=plt.plotfile('data-Large.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval (Adimensional)', 'Number of evaluations'), marker='x', newfig=False, label='0.3 m')

SST=plt.plotfile('data-SST.txt', cols=(0, 2), delimiter=' ', 
                     names=('Time interval (Adimensional)', 'Wall Painted (%)', 'Number of evaluations'), marker='p', newfig=False, label='$\infty$')
 


leg=plt.legend(frameon=False,loc=2)
leg.get_frame().set_alpha(0.5)
# plt.title('Evaluations vs time interval')

#plt.plotfile('AFFG-Ev-paint.txt', 'PSO-Ev-paint.txt', 'PSOFI-Ev-paint.txt', 'SST-Ev-paint.txt', subplots=False, delimiter=' ', newfig=False, names=('Time interval', 'Wall Painted (%)', 'Number of evaluations'), marker='o')

matplotlib.rcParams['ps.useafm'] = True
matplotlib.rcParams['pdf.use14corefonts'] = True
matplotlib.rcParams['text.usetex'] = True
plt.savefig('PAINT-EvInt.pdf')
plt.show()
