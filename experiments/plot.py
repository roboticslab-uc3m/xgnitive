import matplotlib.pyplot as plt 

plt.plotfile('2-40-1000-10-30.txt', delimiter=' ', cols=(1, 0), 
             names=('Fitness', 'Number of Evaluations'))


plt.plotfile('3-0-1000-10-30.txt', delimiter=' ', cols=(1, 0), newfig=False, 
             names=('Fitness', 'Number of Evaluations'))

plt.show()
