i=0
while i<17:
    f = open('Large-PercentageWall.txt', 'r')
    j=0
    max_percentages=[]
    data_array=[]
    for line in f:
        data_array_aux=0
        min_error=1000
        for x in line.split():
            error=abs(i*6.25-float(x))
            if error < min_error:
                data_array_aux=float(x)
                min_error=error
        max_percentages.append(data_array_aux)
        #print data_array
        j+=1
    l=i
    painted_wall=0
    while l< len(max_percentages):
        painted_wall+=max_percentages[l]
        #print painted_wall
        l+=17

    j=j/17
    print j
    print i
    print painted_wall/j

    i+=1
