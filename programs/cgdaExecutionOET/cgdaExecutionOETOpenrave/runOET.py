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

#TODO list:
# 1) Implement here that zeros and ones are read from a txt and send through args. X
# 2) Implement in CgdaExecutionOET that the outputs of paintsquared are send to the txt called before.
# 3) Implement a while that is running until certain condition is achieved. Maybe extract this condition from evMono_ecf_params, just for simplicity.
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
        data_array=[ "0","0", "0","0","0","0", "0","0","0","0","0","0","0","0","0","0"]
	print"Empty Memory"

    sum=0
    for i in data_array:
        sum=sum+int(i)
    if sum==16:
        break

    print data_array

    #Now delete the content in the file
    f.close()
    with open('memoryOET.txt', 'w'): pass

    #args = ("./cgdaExecutionOETOpenrave", "../../programs/cgdaExecutionOET/conf/evMono_ecf_params.xml", "1","0", "0","0","0","0", "1","1","0","0","0","0","0","0","0","0" )
    args = ("mpiexec", "-n", "1", "./cgdaExecutionOETOpenrave", "../../programs/cgdaExecutionOET/conf/evMono_ecf_params.xml", data_array[0],data_array[1], data_array[2],data_array[3],data_array[4],data_array[5], data_array[6],data_array[7],data_array[8],data_array[9],data_array[10],data_array[11],data_array[12],data_array[13],data_array[14],data_array[15] )

    popen = subprocess.Popen(args, stdout=subprocess.PIPE) #Pipe is the standard stdout
    #popen.wait()
    #output = popen.stdout.read()
    #print output
    out, err = popen.communicate()
    errcode = popen.returncode
    #output = popen.stdout.read()
    ###print out
    print"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"

print "Done!!!!!"
