<<<<<<< HEAD
f = open('80-TrajectoryterationsvsEvaluations.txt', 'r')
=======
f = open('20-TrajectoryterationsvsEvaluations.txt', 'r')
>>>>>>> 3290490144497024a582d60da1cb9b9df25e484e
data_string=""
data_array=[]
min_value_ev=10000
for line in f:
    if line.startswith('8'):
	    for x in line.split():
	        data_array.append(float(x))

#print data_array
#The Number of evaluations mean is:
i=3
ev=0
fit=0
j=0
while i <= len(data_array):
    ev+=data_array[i-1]
    if data_array[i-1]<min_value_ev:
	min_value_ev=data_array[i-1]
    fit+=data_array[i]
    j+=1
    i+=4
print j
ev_mean=ev/j
fit_mean=fit/j
print ev_mean
print fit_mean
print "min value"
print min_value_ev

#Lets calculate the variance
i=2
variance=0
while i<=len(data_array):
    variance+=data_array[i]
    i+=4 

print "The variance is: "
print variance/(j^2)
