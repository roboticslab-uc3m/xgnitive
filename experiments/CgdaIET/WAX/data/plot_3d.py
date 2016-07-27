#!/usr/bin/python
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt 
import numpy as np
import time

font = {'family' : 'normal',
        'size'   : 27}

plt.rc('font', **font)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

data = np.genfromtxt('generalized.txt')
xg = data[:,1]
yg = data[:,2]
zg = data[:,3]

data = np.genfromtxt('AFFGPSO-10.txt')
x = data[:,1]
y = data[:,2]
z = data[:,3]

ax.plot(y,x,z, marker='o')
ax.plot(yg,xg,zg, marker='o')
ax.set_xlabel('X (mm)')
ax.set_ylabel('Y (mm)')
ax.set_zlabel('Z (mm)')
ax.set_zlim3d(180,320)

ax.text2D(0.2, 0.85, "C)", transform=ax.transAxes)

fig.show()
fig.savefig('AFFGPSO.pdf')

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

data = np.genfromtxt('PSO.txt')
x = data[:,1]
y = data[:,2]
z = data[:,3]

ax.plot(y,x,z, marker='o')
ax.plot(yg,xg,zg, marker='o')
ax.set_xlabel('X (mm)')
ax.set_ylabel('Y (mm)')
ax.set_zlabel('Z (mm)')
ax.set_zlim3d(180,320)

ax.text2D(0.2, 0.85, "B)", transform=ax.transAxes)

fig.show()
fig.savefig('PSO.pdf')

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

data = np.genfromtxt('SST.txt')
x = data[:,1]
y = data[:,2]
z = data[:,3]

ax.plot(y,x,z, marker='o')
ax.plot(yg,xg,zg, marker='o')
ax.set_xlabel('X (mm)')
ax.set_ylabel('Y (mm)')
ax.set_zlabel('Z (mm)')
ax.set_zlim3d(180,320)


ax.text2D(0.2, 0.85, "A)", transform=ax.transAxes)

fig.show()
fig.savefig('SST.pdf')

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

data = np.genfromtxt('FIPSO.txt')
x = data[:,1]
y = data[:,2]
z = data[:,3]

ax.plot(y,x,z, marker='o')
ax.plot(yg,xg,zg, marker='o')
ax.set_xlabel('X (mm)')
ax.set_ylabel('Y (mm)')
ax.set_zlabel('Z (mm)')
ax.set_zlim3d(180,320)

ax.text2D(0.2, 0.85, "D)", transform=ax.transAxes)

fig.show()
fig.savefig('FIPSO.pdf')

time.sleep(100)
