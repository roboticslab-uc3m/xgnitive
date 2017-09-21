#!/usr/bin/python

## numpy is used for creating fake data
import numpy as np 
import matplotlib as mpl 
import matplotlib.pyplot as plt 

#import matplotlib

#matplotlib.rcParams['ps.useafm'] = True
#matplotlib.rcParams['pdf.use14corefonts'] = True
#matplotlib.rcParams['text.usetex'] = True

font = {'family' : 'Helvetica',
        'size'   : 36}

plt.rc('font', **font)


fte=[263,345,209]
iet=[115,148,166]
oet=[3.9810779095, 3.8074719906, 4.1091489792, 3.8754820824, 3.8158481121, 3.9638550282, 3.8815281391, 3.6324350834, 3.8728020191, 3.6472148895, 3.8645160198, 3.8971121311,3.3999831677, 3.415995121, 3.7009780407, 3.8986778259, 3.6791219711, 4.4722249508, 4.4507529736, 5.6388869286, 3.6090362072, 3.954447031, 3.3659791946, 3.9020190239, 3.7741010189, 4.011754036, 4.0648510456, 7.3000378609, 3.9543938637, 3.9721271992, 3.9382181168, 3.9818081856, 3.734610080, 3.5969200134, 3.9504361153, 5.4207720757]


data_to_plot = [fte,iet,oet]

fig = plt.figure(1, figsize=(30, 20))

# Create an axes instance
ax = fig.add_subplot(111)

ax.set_xticklabels(['FTE', 'IET', 'OET'])

ax.set_ylabel('Real Iteration Time (s)')

# Create the boxplot
bp = ax.boxplot(data_to_plot, widths = 0.9)



plt.show()






