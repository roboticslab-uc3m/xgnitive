README.md
=========
0. Check hardware (iron f/t sensor, etc).
1. Start teoBase: launchManipulation, Jr3, leftArm BasicCartesianControl, and rightArm BasicCartesianControl. Check everything working with teoTools.
2. Start teoTools: yarp rpc /teo/leftArm/rpc:i, yarp rpc /teo/rightArm/rpc:i.
3. Set leftArm to 0, rightArm to (-2.267151 -14.217926 23.989454 84.710014 30.667837 -61.933228 0.0).
4. Send a [gcmp] to leftArm and rightArm.
5. Start icra2018-dump: play and connect.
6. Move robot.
7. Disconnect and stop icra2018-dump to stop recording.
