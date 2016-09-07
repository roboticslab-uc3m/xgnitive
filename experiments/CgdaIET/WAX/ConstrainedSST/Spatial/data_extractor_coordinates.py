import numpy
import math
i=0
while i<17:
    fx = open('Intermediate-X.txt', 'r')
    fy = open('Intermediate-Y.txt','r')
    fz = open('Intermediate-Z.txt','r')
    i=0
    max_percentages=[]
    data_array_x=[]
    data_array_y=[]
    data_array_z=[]
    for line in fx:
        data_array_x.append([])
        for x in line.split():
            data_array_x[i].append(float(x))        
        i+=1
    
    i=0
    
    for line in fy:
        data_array_y.append([])
        for x in line.split():
            data_array_y[i].append(float(x))        
        i+=1
    
    i=0

    for line in fz:
        data_array_z.append([])
        for x in line.split():
            data_array_z[i].append(float(x))        
        i+=1
    
    #print data_array_x

    #In the next loop we search for the points (x,y,z) that have the lowest
    #error in each iteration
    i=0
    j=0
    Npoint=0
    results=numpy.zeros((9,3))
    while i<len(data_array_x):
        j=0
        aux_results=numpy.zeros((9,3))
        min_error=1000

        if Npoint==9:
            Npoint=0
       
        while j<len(data_array_x[i]):
            if Npoint==0:
                error=math.sqrt(pow((461.238-data_array_x[i][j]),2)+pow((-502.726-data_array_y[i][j]),2)+pow((303.978-data_array_z[i][j]),2))
                if error<min_error:
			        aux_results[0][0]=data_array_x[i][j]
			        aux_results[0][1]=data_array_y[i][j]
			        aux_results[0][2]=data_array_z[i][j]
			        min_error=error
            if Npoint==1:
                error=math.sqrt(pow((457.608-data_array_x[i][j]),2)+pow((-516.857-data_array_y[i][j]),2)+pow((303.026-data_array_z[i][j]),2))
                if error<min_error:
                    aux_results[1][0]=data_array_x[i][j]
                    aux_results[1][1]=data_array_y[i][j]
                    aux_results[1][2]=data_array_z[i][j]
                    min_error=error
            
            if Npoint==2:
                error=math.sqrt(pow((419.358-data_array_x[i][j]),2)+pow((-588.84-data_array_y[i][j]),2)+pow((298.77-data_array_z[i][j]),2))
                if error<min_error:
			        aux_results[2][0]=data_array_x[i][j]
			        aux_results[2][1]=data_array_y[i][j]
			        aux_results[2][2]=data_array_z[i][j]
			        min_error=error

            if Npoint==3:
                error=math.sqrt(pow((316.952-data_array_x[i][j]),2)+pow((-620.8-data_array_y[i][j]),2)+pow((293.525-data_array_z[i][j]),2))
                if error<min_error:
			        aux_results[3][0]=data_array_x[i][j]
			        aux_results[3][1]=data_array_y[i][j]
			        aux_results[3][2]=data_array_z[i][j]
			        min_error=error
            if Npoint==4:
                error=math.sqrt(pow((235.19-data_array_x[i][j]),2)+pow((-554.147-data_array_y[i][j]),2)+pow((292.914-data_array_z[i][j]),2))
                if error<min_error:
			        aux_results[4][0]=data_array_x[i][j]
			        aux_results[4][1]=data_array_y[i][j]
			        aux_results[4][2]=data_array_z[i][j]
			        min_error=error
            if Npoint==5:
                error=math.sqrt(pow((283.267-data_array_x[i][j]),2)+pow((-448.075-data_array_y[i][j]),2)+pow((300.955-data_array_z[i][j]),2))
                if error<min_error:
			        aux_results[5][0]=data_array_x[i][j]
			        aux_results[5][1]=data_array_y[i][j]
			        aux_results[5][2]=data_array_z[i][j]
			        min_error=error
            if Npoint==6:
                error=math.sqrt(pow((408.68-data_array_x[i][j]),2)+pow((-435.327-data_array_y[i][j]),2)+pow((306.882-data_array_z[i][j]),2))
                if error<min_error:
			        aux_results[6][0]=data_array_x[i][j]
			        aux_results[6][1]=data_array_y[i][j]
			        aux_results[6][2]=data_array_z[i][j]
			        min_error=error
            if Npoint==7:
                error=math.sqrt(pow((461.545-data_array_x[i][j]),2)+pow((-484.298-data_array_y[i][j]),2)+pow((304.275-data_array_z[i][j]),2))
                if error<min_error:
			        aux_results[7][0]=data_array_x[i][j]
			        aux_results[7][1]=data_array_y[i][j]
			        aux_results[7][2]=data_array_z[i][j]
			        min_error=error
            if Npoint==8:
                error=math.sqrt(pow((463.079-data_array_x[i][j]),2)+pow((-500.373-data_array_y[i][j]),2)+pow((303.831-data_array_z[i][j]),2))
                if error<min_error:
			        aux_results[8][0]=data_array_x[i][j]
			        aux_results[8][1]=data_array_y[i][j]
			        aux_results[8][2]=data_array_z[i][j]
			        min_error=error
            j+=1
        print aux_results
        results+=aux_results        
        i=i+1
        print i
        Npoint+=1
    results=numpy.divide(results,50)        
    print results
    print points

