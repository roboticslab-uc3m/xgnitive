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
# 1) Implement here that zeros and ones are read from a txt and send through args.
# 2) Implement in CgdaExecutionOET that the outputs of paintsquared are send to the txt called before
# 3) Implement a while that is running until certain condition is achieved. Maybe extract this condition from evMono_ecf_params, just for simplicity.
args = ("./cgdaExecutionOETOpenrave", "../../programs/cgdaExecutionOET/conf/evMono_ecf_params.xml", "1","0", "0","0","0","0", "1","1","0","0","0","0","0","0","0","0" )

popen = subprocess.Popen(args, stdout=subprocess.PIPE) #Pipe is the standard stdout
popen.wait()
output = popen.stdout.read()
print output
