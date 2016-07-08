import matplotlib.pyplot as plt 

#plt.plotfile('2-40-1000-10-30.txt', delimiter=' ', cols=(1, 0), 
#             names=('Fitness', 'Number of Evaluations'))


plt.plotfile('4-Fuzzy-500I-10-30.txt', delimiter=' ', cols=(1, 0), newfig=False, 
             names=('Fitness', 'Number of Evaluations'))

plt.show()
