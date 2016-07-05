f = open('TVE-100IT-PSOFI-40%-10PART-10MAX.txt', 'r')
data_string=""
data_array=[]
for line in f:
    if line.startswith('16'):
	data_line= line
	data_string+=data_line
data_array=[int(s) for s in data_string.split() if s.isdigit()]
#print data_array
#The Number of evaluations mean is:
print data_array
i=2
mean=0
j=1
while i <= len(data_array):
    mean+=data_array[i]
    j+=1
    i+=3
print j
mean=mean/j
print mean
