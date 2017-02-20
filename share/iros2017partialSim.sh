#!/bin/sh
YARP_PORT_PREFIX=/0 /usr/bin/python $HOME/repos/openrave-yarp-plugins/examples/openrave-YarpRobotPaintSquares.py &
YARP_PORT_PREFIX=/1 /usr/bin/python $HOME/repos/openrave-yarp-plugins/examples/openrave-YarpRobotPaintSquares.py &

