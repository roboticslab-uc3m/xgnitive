# Installation (Ubuntu 14.04)

First install dependencies:

## ECF (1.4.1)
  
```bash
#In a new terminal
cd /tmp/
wget https://sourceforge.net/projects/ecfproject/files/ECF_1.4.1/ECF_1.4.1.tar.gz/download -O ECF_1.4.1.tar.gz
tar -zxvf ECF_1.4.1.tar.gz
cd ECF_1.4.1
wget http://ecf.zemris.fer.hr/ecf_install.sh
chmod +x ecf_install.sh
./ecf_install.sh
```

## CMake

```bash
sudo apt-get install cmake
sudo apt-get install cmake-curses-gui  # Recommended, as it contains ccmake.
```

## Yarp 

```bash
sudo apt-get install build-essential libace-dev subversion git
sudo apt-get install libgsl0-dev  # Needed for creating YARP lib_math we use for kinematics
sudo apt-get install libgtkmm-2.4-dev  # Needed for YARP GUIs: yarpview, gyarpmanager
sudo apt-get install libjpeg8-dev   # Needed for mjpeg_carrier for ravebot cam on web
cd  # go home
mkdir -p repos; cd repos  # make $HOME/repos if it doesn't exist; then, enter it
git clone https://github.com/robotology/yarp
cd yarp; mkdir build; cd build
cmake .. -DCREATE_LIB_MATH=ON -DCREATE_GUIS=ON -DCREATE_OPTIONAL_CARRIERS=ON -DENABLE_yarpcar_mjpeg_carrier=ON -DYARP_USE_QT5=OFF  # YARP_USE_QT5 can induce errors as of 02/2016.
make -j3;  sudo make install; sudo ldconfig; cd  # go home
```

## Aquila
```bash
sudo apt-get install libsfml-dev
cd  # go home
mkdir -p repos; cd repos  # make $HOME/repos if it doesn't exist; then, enter it
git clone git://github.com/zsiciarz/aquila.git
cd aquila; mkdir build; cd build
cmake ..
make -j3;  sudo make install; sudo ldconfig; cd  # go home
```

## orocos_kdl

```bash
mkdir -p repos; cd repos  # make $HOME/repos if it doesn't exist; then, enter it
git clone https://github.com/orocos/orocos_kinematics_dynamics.git
cd orocos_kinematics_dynamics/orocos_kdl
mkdir -p build; cd build
cmake ..
make
sudo make install
```

## pygame (only for "paintSquaresOnScreen" program)
```bash
sudo apt-get install python-pygame
```

##Install XGNITIVE

For additional xgnitive options use ccmake instead of cmake.

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