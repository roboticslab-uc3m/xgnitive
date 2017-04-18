# Installation 

First install dependencies:

- [Install Boost](https://github.com/roboticslab-uc3m/installation-guides/blob/master/install_boost.md)
- [Install CMake](https://github.com/roboticslab-uc3m/installation-guides/blob/master/install_cmake.md)
- [Install PCL](https://github.com/roboticslab-uc3m/installation-guides/blob/master/install_pcl.md)
- [Install ECF](https://github.com/roboticslab-uc3m/installation-guides/blob/master/install_ecf.md)
- [Install YARP with OpenNI2DeviceServer and Python bindings](https://github.com/roboticslab-uc3m/installation-guides/blob/master/install_yarp.md)
- [Install OpenRAVE](https://github.com/roboticslab-uc3m/installation-guides/blob/master/install_openrave.md)
- [Install Aquila](https://github.com/roboticslab-uc3m/installation-guides/blob/master/install_aquila.md )
- [Install KDL](https://github.com/roboticslab-uc3m/installation-guides/blob/master/install_kdl.md)
- [Install OpenCV](https://github.com/roboticslab-uc3m/installation-guides/blob/master/install_opencv.md)
- [Install Pygame]( https://github.com/roboticslab-uc3m/installation-guides/blob/master/install_pygame.md)
- [Install teo-openrave-models](https://github.com/roboticslab-uc3m/teo-openrave-models)

## Install XGNITIVE (working on all tested Ubuntu versions)

```bash
cd  # go home
mkdir -p repos; cd repos  # make $HOME/repos if it doesn't exist; then, enter it
git clone https://github.com/roboticslab-uc3m/xgnitive.git  # Download xgnitive software from the repository
cd xgnitive; mkdir build; cd build; cmake ..  # Configure the xgnitive software
make  # Compile
sudo make install  # Install :-)
cp ../scripts/gnome/xgnitive.desktop $HOME/Desktop
```
For additional options use ccmake instead of cmake.
