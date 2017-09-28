# Experiment 1

### raw folder
The .log files of this directory contain logs produced by teaching the robot two different actions: "iron" and "paint". Recorded using https://github.com/roboticslab-uc3m/xgnitive/tree/ac0c134e686300d91b42f4889318c3bfed22bbe3/share/icra2018.

The data delimiter used is the space character (' '), and decimals are separated by the point character ('.'). For each of the actions three different folders are presented in the dataset.

#####jr3 folder

This folder contains a data.log file with the data obtained from the force sensor presented in the right end-effector of the robot.

The data is set in the following order, in plain text and no specific header:

- YARP id
- Timestamp [s]
- Fx (Force experienced in the right end-effector of the robot in the X axis [N*])
- Fy (Force experienced in the right end-effector of the robot in the Y axis [N*])
- Fz (Force experienced in the right end-effector of the robot in the Z axis [N*])
- Tx (Torque experienced in the right end-effector of the robot around the X axis [N.m])
- Ty (Torque experienced in the right end-effector of the robot around the Y axis [N.m])
- Tz (Torque experienced in the right end-effector of the robot around the Z axis [N.m])

*Although experiments have shown this is the unit used by the end-effector, in the sensor documentation pounds is specified as the used unit.
 
#####paintSquaresOnScreen folder

This folder contains a data.log file with the information of the percentage of wall painted at each time-step.

The data is set in the following order, in plain text and no specific header:

- YARP id
- Timestamp [s]
- Percentage of painted wall [%]

#####teo folder

This folder contains two different folders (rightArm and leftArm). These folders contains the kinematic information of the right and left end-effector respectively.

The data is set in the following order, in plain text and no specific header:

- YARP id
- Timestamp [s]
- X ( X coordinate of the position of the end-effector [m])
- Y ( Y coordinate of the position of the end-effector [m])
- X ( Z coordinate of the position of the end-effector [m])
- axisX ( x component of the normalized quaternion [degrees])
- axisY ( y component of the normalized quaternion [degrees])
- axisZ ( z component of the normalized quaternion [degrees])
- Angle ( w component of the normalized quaternion [degrees])
