# How-to paintSquaresOnScreen

This application allow you to use the kinect to detect the position of your hand or a brush, and use it to paint squares on a screen. To use this application make sure you have installed OpenNI2DeviceServer from the [YARP installation manual](https://github.com/roboticslab-uc3m/installation-guides/blob/c391d11f6bb624458111bd6655cfcb9109e7952a/install-yarp.md). It also needs to have installed the [vision package](https://github.com/roboticslab-uc3m/vision).

In order to use this application, perform the next steps:

```bash
#Init software for kinect
yarpdev --device OpenNI2DeviceServer
cd ~/repos/vision/build/bin; ./voxelOccupancyDetection
#Init paint screen
cd ~/repos/xgnitive/programs/paintSquaresOnScreen; ./paintSquaresOnScreen.py
#Connect
yarp connect /voxelOccupancyDetection/state:o /voxelOccupancyDetection/state:i
```

