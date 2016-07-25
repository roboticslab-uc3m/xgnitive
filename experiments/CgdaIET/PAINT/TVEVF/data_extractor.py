f = open('TVEVF-100IT-PSOFI-55%-10PART-10MAX.txt', 'r')
data_string=""
data_array=[]
for line in f:
    if line.startswith('16'):
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
    fit+=data_array[i]
    j+=1
    i+=4
print j
ev_mean=ev/j
fit_mean=fit/j
print ev_mean
print fit_mean
