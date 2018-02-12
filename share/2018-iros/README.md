README.md
=========

# Paint record
0. Check hardware (robot, etc).
1. From Desktop `TEO Apps yarpmanager` -> `teoBase`: launchManipulation, launchLocomotion, leftArm BasicCartesianControl, and leftArm BasicCartesianControl. Check everything working with teoTools.
2. Start paintSquaresOnScreen-real App.
3. Start teoTools: yarp rpc /teo/leftArm/rpc:i, yarp rpc /teo/rightArm/rpc:i.
4. Set leftArm to ¿¿¿¿(-2.267151 -14.217926 23.989454 84.710014 30.667837 -61.933228 0.0)????.
4. Send a [gcmp] to leftArm.
5. Start 2018-iros-paint-record App: play and connect.
6. Move robot.
7. Disconnect and stop 2018-iros-paint-record App to stop recording.

# Iron record
0. Check hardware (iron f/t sensor, etc).
1. Start teoBase: launchManipulation, Jr3, and rightArm BasicCartesianControl. Check everything working with teoTools.
