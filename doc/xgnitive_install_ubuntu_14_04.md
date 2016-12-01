# Installation (Ubuntu 14.04)

First install dependencies:
- [Install ECF](https://github.com/roboticslab-uc3m/installation-guides/blob/209f6f21fb4064c19ac6f8263c87c769a63488b1/install_ecf.md)
- [Install CMake] (https://github.com/roboticslab-uc3m/installation-guides/blob/209f6f21fb4064c19ac6f8263c87c769a63488b1/install_cmake.md)
- [Install YARP] ( https://github.com/roboticslab-uc3m/installation-guides/blob/209f6f21fb4064c19ac6f8263c87c769a63488b1/install_yarp.md)
- [Install Aquila] ( https://github.com/roboticslab-uc3m/installation-guides/blob/209f6f21fb4064c19ac6f8263c87c769a63488b1/install_aquila.md )
- [Install Orocos_kdl] ( https://github.com/roboticslab-uc3m/installation-guides/blob/209f6f21fb4064c19ac6f8263c87c769a63488b1/install_orocos_kdl.md)
- [Install pygame] ( https://github.com/roboticslab-uc3m/installation-guides/blob/209f6f21fb4064c19ac6f8263c87c769a63488b1/install_pygame.md)

##Install XGNITIVE

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
