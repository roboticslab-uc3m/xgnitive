# CGDA IET Execution

Right now two different actions are available for it's execution using IET with CGDA. These actions are the "paint" and "iron" actions.

## IET + Paint action

Run the following commands in different terminals.

```
yarpserver 
cd ~/repos/openrave-yarp-plugins/examples; python openrave-YarpRobotPaintSquares.py 
cd ~/repos/xgnitive/build/bin; ./cgdaExecutionIET
```

## IET + Ironning

Run the following commands in different terminals.

```
yarpserver
cd ~/repos/openrave-yarp-plugins/examples; python openrave-YarpForceEstimator.py
yarp rpc /teoSim/trunk/rpc:i
set pos 0 45 #Same terminal as the one we executed yarp rpc /teoSim/trunk/rpc:i
cd ~/repos/openrave-yarp-plugins/examples; ./cgdaExecutionIET
```
