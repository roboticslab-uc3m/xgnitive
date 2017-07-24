README.md
=========
0. Check hardware (iron f/t sensor, etc).
1. Start teoBase: launchManipulation, Jr3, leftArm BasicCartesianControl, and rightArm BasicCartesianControl. Check everything working with teoTools.
2. Start teoTools: yarp rpc /teo/leftArm/rpc:i, yarp rpc /teo/rightArm/rpc:i.
3. Send a [gcmp] to each of the above (2).
4. Start icra2018-dump: play and connect.
5. Move robot.
6. Disconnect and stop icra2018-dump to stop recording.
