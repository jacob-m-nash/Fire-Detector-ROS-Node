# Fire-Detector-ROS-Node
 Ros node implantation of Fire-Detector
## Setup (Ubuntu 20.04)
### Install ROS Noetic
Taken from [here](http://wiki.ros.org/noetic/Installation/Ubuntu) (date accessed 23/05/2023)

#### Setup your sources.list
```
sudo apt install curl # if you haven't already installed curl
```
```
curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -
```
#### Installation
```
sudo apt update # make sure your Debian package index is up-to-date
```
```
sudo apt install ros-noetic-desktop-full
```
#### Environment setup
```
echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
```
```
source ~/.bashrc
```
### Build OpenCV
Taken from [here](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html) (date accessed 23/05/2023)
#### Install minimal prerequisites
```
sudo apt update && sudo apt install -y cmake g++ wget unzip
```
#### Create folder for source code
```
mkdir -p OpenCV && cd OpenCV 
```
#### Download and unpack sources
```
wget -O opencv.zip https://github.com/opencv/opencv/archive/4.x.zip
```
```
unzip opencv.zip
```
```
mkdir -p build && cd build
```
#### Configure
```
cmake  ../opencv-4.x
```
#### Build 
```
cmake --build .
```
### Install Boost
Taken from [here](https://www.boost.org/doc/libs/1_82_0/more/getting_started/unix-variants.html) (date accessed 23/05/2023)
#### Download Boost
```
wget https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.bz2
```
#### Extract and Execute
```
tar --bzip2 -xf boost_1_82_0.tar.bz2
``` 
### Download and build darkent_ros
Taken from [here](https://github.com/leggedrobotics/darknet_ros/blob/master/README.md) (date accessed 23/05/2023)
#### Download source code
```
cd catkin_workspace/src
```
```
git clone --recursive https://github.com/leggedrobotics/darknet_ros.git
```
#### Build
```
catkin build darknet_ros -DCMAKE_BUILD_TYPE=Release
```
### TODO: Download and move example yolo model weights
### Download Other Dependencies
usb-cam (for testing)
```bash
sudo apt install ros-noetic-usb-cam
``` 
## Build everything else
```
catkin build
```
### TODO: Download and move fire-detector model weights
## Launch
```ROS
roslaunch fire_detector fire_detector.launch 
```