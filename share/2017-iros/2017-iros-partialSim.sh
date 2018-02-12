#!/bin/sh

YARP_PORT_PREFIX=/0 /usr/bin/python $HOME/repos/openrave-yarp-plugins/examples/openrave-YarpRobotPaintSquares.py &

YARP_PORT_PREFIX=/1 /usr/bin/python $HOME/repos/openrave-yarp-plugins/examples/openrave-YarpRobotPaintSquares.py &

# test 1
$HOME/repos/xgnitive/build/bin/cgdaExecutionOETOpenrave $HOME/repos/xgnitive/programs/cgdaExecutionOET/conf/evMono_ecf_params.xml 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
# run all
cd $HOME/repos/xgnitive/build/bin/
rm memoryOET.txt
touch memoryOET.txt
python ./runOET.py

