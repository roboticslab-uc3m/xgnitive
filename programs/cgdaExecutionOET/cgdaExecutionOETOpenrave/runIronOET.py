#!/usr/bin/env python

'''
runOET.py program

Authors: Raul Fernandez-Fernandez; Juan G. Victores

Universidad Carlos III de Madrid

'''

## @ingroup xgnitive_programs
 # \defgroup paintSquaresOnScreen
 #
 # @brief This program is in charge of performing the execution of the cgda system using OET until the task is completed.
 #
 # @section paintSquaresOnScreen_legal Legal
 #
 # Copyright: 2017 (C) Universidad Carlos III de Madrid
 #
 # Author: raulfdzbis, jgvictores
 #
 # CopyPolicy: This program is free software; you can redistribute it and/or modify
 # it under the terms of the GNU Lesser General Public License 3.0 or later
 #
 # <hr>
 #
 # This file can be edited at runOET

import subprocess
import time

#TODO list:
# 1) Implement here that zeros and ones are read from a txt and send through args. X
# 2) Implement in CgdaExecutionOET that the outputs of paintsquared are send to the txt called before.
# 3) Implement a while that is running until certain condition is achieved. Maybe extract this condition from evMono_ecf_params, just for simplicity.
Gen=0
fout= open('percentageOET.txt', 'a')

fout2= open('DataOET++.txt', 'a')
fout2.write("\n")
fout2.write("I: ")
fout2.close()

Nsquares=64

while 1:
    f = open('memoryOET.txt', 'r')
    data_array=[] #Array of strings
    suma = 0

    #Read all the numbers in memoryOET.txt
    for line in f:
        for x in line.split():
            data_array.append(x)
            if x == "1":
                suma += 1
        print("sum:",suma)
    if not data_array:
	print"Empty Memory"
        for x in range(Nsquares):
            data_array.append("0")
            
    sum=0
    for i in data_array:
        sum=sum+int(i)
    
    #Output percentage
    print sum
    percentage=(sum/float(Nsquares))*100.0
    percentage=str(percentage)
    print percentage
    fout.write(percentage)
    fout.write(" ")
    if sum==Nsquares or Gen==30:
	fout2= open('DataOET++.txt', 'a')
	fout2.write("\n")
	fout2.write("I: ")
        fout2.close()
        break

    #Now delete the content in the file
    f.close()
    with open('memoryOET.txt', 'w'): pass

    #args = ("./cgdaExecutionOETOpenrave", "../../programs/cgdaExecutionOET/conf/evMono_ecf_params.xml", "1","0", "0","0","0","0", "1","1","0","0","0","0","0","0","0","0" )

    start_time = time.time()

    #args = ("mpiexec", "-n", "1", "./cgdaExecutionOETOpenrave", "../../programs/cgdaExecutionOET/conf/evMono_ecf_params.xml")

    print data_array

    popen = subprocess.Popen(["mpiexec", "-n", "1", "./cgdaExecutionOETOpenrave", "../../programs/cgdaExecutionOET/conf/evMono_ecf_params.xml"] + data_array, stdout=subprocess.PIPE) #Pipe is the standard stdout
    #popen.wait()
    #output = popen.stdout.read()
    #print output
    out, err = popen.communicate()
    errcode = popen.returncode
    #output = popen.stdout.read()
    ###print out

    #Output time
    elapsed_time=time.time() - start_time
    elapsed_time=str(elapsed_time)
    fout.write(elapsed_time)
    fout.write("\n")

    print"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    Gen=Gen+1;

print "Done!!!!!"

