# Installation 

First install dependencies:
- [Install Boost] (https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_boost.md)
- [Install CMake] (https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_cmake.md)
- [Install PCL] ( https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_pcl.md)
- [Install ECF](https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_ecf.md)
- [Install YARP] ( https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_yarp.md)
- [Install OpenRAVE] ( https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_openrave.md)
- [Install Aquila] ( https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_aquila.md )
- [Install KDL] ( https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_kdl.md)
- [Install Pygame] ( https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_pygame.md)

##Install XGNITIVE (working on all tested versions)

```bash
cd  # go home
mkdir -p repos; cd repos  # make $HOME/repos if it doesn't exist; then, enter it
git clone https://github.com/roboticslab-uc3m/xgnitive.git  # Download xgnitive software from the repository
cd xgnitive; mkdir build; cd build; cmake ..  # Configure the xgnitive software
make  # Compile
sudo make install  # Install :-)
cp ../scripts/gnome/xgnitive.desktop $HOME/Desktop
```
For additional xgnitive options use ccmake instead of cmake.
