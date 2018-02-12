README.md
=========

# Paint record
0. Check hardware (robot, etc).
1. From Desktop `TEO Apps yarpmanager` -> `teoBase`: launchManipulation, launchLocomotion, leftArm BasicCartesianControl, and leftArm BasicCartesianControl. Check everything working with teoTools.
2. Start paintSquaresOnScreen-real App.
3. Via `TEO Apps yarpmanager` -> `teoTools`: `yarp rpc /teo/leftArm/rpc:i`, `yarp rpc /teo/rightArm/rpc:i`
4. `set poss (0 0 0 0 0 0)`. Can use `[set] [icmd] [cmod] 0 [torq]` to set individual as in [yarp-devices-tricks](https://github.com/roboticslab-uc3m/yarp-devices/blob/develop/doc/yarp-devices-tricks.md).
4. Send a `[gcmp]` to leftArm BasicCartesianControl.
5. Start 2018-iros-paint-record App: play and connect.
6. Move robot.
7. Disconnect and stop 2018-iros-paint-record App to stop recording.

# Iron record
0. Check hardware (iron f/t sensor, etc).
1. Start teoBase: launchManipulation, Jr3, and rightArm BasicCartesianControl. Check everything working with teoTools.
