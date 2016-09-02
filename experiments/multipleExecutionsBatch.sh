#!/bin/sh
clear 
echo "Hi there! We are entering Batch mode: Performing multiple runs.............................................................."
Number_of_iterations=100 #Set here the number of iterations you want to perform with the cgda system.
i=1
echo "The number of iterations that will be perfomed will be $Number_of_iterations"
#sleep 2
cd ../build/bin
while [ $i -le $Number_of_iterations ]
do
    ./cgdaExecutionIET
    echo "This is the iteration number $i ........................................................................................"
    i=$((i+1))
    wait
done
exit 0
