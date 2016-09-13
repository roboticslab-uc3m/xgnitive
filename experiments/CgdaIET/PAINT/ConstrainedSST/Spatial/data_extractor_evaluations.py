time=0
while time < 17:
	f = open('Large-TrajectoryterationsvsEvaluations.txt', 'r')
	data_string=""
	data_array=[]
	min_value_ev=10000
	ev_mean=0
	fit_mean=0
	for line in f:
	    if line.startswith(str(time)):
	    #if line.startswith('1 '):
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
	    if data_array[i]>5000:	    
		fit+=843
	    else: 
		fit+=data_array[i]
	    j+=1
	    i+=4
	print j
	ev_mean=ev/j
	fit_mean=fit/j
	print ev_mean
	print fit_mean
	print '++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++'
	time=time+1
	


#Lets calculate the variance
i=2
variance=0
while i<=len(data_array):
    variance+=data_array[i]
    i+=4 

print "The variance is: "
print variance/(j^2)
