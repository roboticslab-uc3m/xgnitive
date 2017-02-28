#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np
font = {'family' : 'serif',
        'weight' : 'ultralight',
        'size'   : 18}

plt.rc('font', **font)


#FTE (A script to extract this values would be so much better)
FTE=[10, 16.88, 22.5, 28.13, 34.38, 39.38, 45, 50.63, 55.63, 61.88, 66.25, 73.13, 76.88, 81.88,	87.5, 90, 90]


#IET
IET=[0, 6.25, 11.88, 18.13, 24.38, 30, 36.88, 42.5, 49.38, 55.63, 61.88, 68.13, 74.38, 78.13, 80, 82.5, 83.75]



#OET
OET=[0,6.25,11.88,18.13,25,31.25,36.88,43.75,50,56.25,62.5,68.75,75,81.25,87.5,91.88, 92.5]

#Generalized
Gen=[0,6.25,12.5,18.75,25,31.25,37.5,43.75,50,56.25,62.5,68.75,75,81.25,87.5,93.75,100]

#Time intervals
x=[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]

#plt.plot(Gen, y, marker='+',FTE,y, marker='o', IET, y, marker='s', OET, y, marker='^')
#plt.plot(x, Gen, '+', x, FTE, x, IET, x, OET)
plt.plot(x, Gen, marker='.', label='Generalized')
plt.plot(x, FTE, marker='o', label='FTE')
plt.plot(x, IET, marker='s', label='IET')
plt.plot(x, OET, marker='^', label='OET')
plt.legend(loc=2)
plt.ylabel('Wall Painted (%)')
plt.xlabel('Time interval')

plt.savefig('Trajectories.pdf')
plt.show()



