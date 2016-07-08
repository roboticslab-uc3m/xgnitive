f = open('PercentageWall-TEFP-100IT-AFFG-10PART-10MAX.txt', 'r')
data_string=""
i=1
j=0
max_percentages=[]
for line in f:
    if (i%17) == 0:
	data_array=[]
	for x in line.split():
	    data_array.append(float(x))
	max_percentages.append(max(data_array))
	#print data_array
	j+=1
    i+=1

#print max_percentages
#The mean of the percentage of wall painted is:
print j
print sum(max_percentages)/j
