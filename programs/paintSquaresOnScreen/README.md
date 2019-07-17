# How-to paintSquaresOnScreen

This application allow you to use the kinect to detect the position of your hand/brush and use it to paint squares on a screen. 

## Dependencies
- [YARP with depthCamera](https://github.com/roboticslab-uc3m/installation-guides/blob/78e277815c8b37859b11c73b70c35fc860429ad2/install-yarp.md)
- [vision package](https://github.com/roboticslab-uc3m/vision)

## Usage

```bash
#Init software for kinect
cd; cd repos/vision/share/sensors/conf/
yarpdev --from asus-xtion.ini
cd ~/repos/vision/build/bin; ./voxelOccupancyDetection
#Init paint screen
cd ~/repos/xgnitive/programs/paintSquaresOnScreen; ./paintSquaresOnScreen.py
#Connect
yarp connect /voxelOccupancyDetection/state:o /paintSquaresOnScreen/voxelOccupancyDetection/state:i
```

