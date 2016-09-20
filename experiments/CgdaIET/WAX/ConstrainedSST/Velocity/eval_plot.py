#!/usr/bin/python
import matplotlib.pyplot as plt 
import matplotlib

font = {'family' : 'normal',
        'size'   : 16}

plt.rc('font', **font)

Low1=plt.plotfile('data-5.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval (Adimensional)', 'Number of evaluations'), marker='s', label='5 deg/iter')

Low2=plt.plotfile('data-10.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval (Adimensional)', 'Number of evaluations'), marker='+', newfig=False, label='10 deg/iter')

Low3=plt.plotfile('data-20.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval (Adimensional)', 'Number of evaluations'), marker='o', newfig=False, label='20 deg/iter')

Mid=plt.plotfile('data-60.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval (Adimensional)', 'Number of evaluations'), marker='^', newfig=False, label='60 deg/iter')

High=plt.plotfile('data-80.txt', cols=(0, 1), delimiter=' ', 
                     names=('Time interval (Adimensional)', 'Number of evaluations'), marker='x', newfig=False, label='80 deg/iter')

SST=plt.plotfile('data-SST.txt', cols=(0, 4), delimiter=' ', 
                     names=('Time interval (Adimensional)', 'X','Y','Z', 'Number of evaluations'), newfig=False, marker='p', label='$\infty$')
  


leg=plt.legend(frameon=False,loc=2)
leg.get_frame().set_alpha(0.5)
# plt.title('Evaluations vs time interval')

#plt.plotfile('AFFG-Ev-paint.txt', 'PSO-Ev-paint.txt', 'PSOFI-Ev-paint.txt', 'SST-Ev-paint.txt', subplots=False, delimiter=' ', newfig=False, names=('Time interval', 'Wall Painted (%)', 'Number of evaluations'), marker='o')

matplotlib.rcParams['ps.useafm'] = True
matplotlib.rcParams['pdf.use14corefonts'] = True
matplotlib.rcParams['text.usetex'] = True
plt.savefig('WAX-EvInt-Vel.pdf')
plt.show()
