#!/usr/bin/python
import matplotlib.pyplot as plt 

font = {'family' : 'normal',
        'size'   : 18}

plt.rc('font', **font)

plt.plotfile('IET-Paint-Percentage.txt', delimiter=' ', cols=(0, 1), 
                     names=('Time interval', 'Wall Painted (%)'), marker='o', label='IET')

plt.plotfile('OET-Paint-Percentage.txt', delimiter=' ', cols=(0, 1), newfig=False,
                     names=('Time interval', 'Wall Painted (%)'), marker='^', label='OET')

plt.plotfile('FTE-Paint-Percentage.txt', delimiter=' ', cols=(0, 1), newfig=False, 
                     names=('Time interval', 'Wall Painted (%)'), marker='x', label='FTE')

plt.plotfile('Generalized.txt', delimiter=' ', cols=(0, 1), newfig=False,
                     names=('Time interval', 'Wall Painted (%)'), marker='s', label='Generalized')

plt.legend(loc=2)

plt.savefig('Gen-paint.pdf')

plt.show()
