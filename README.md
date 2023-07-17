# Fire-Detector-ROS-Node
 Ros node implantation of Fire-Detector
## Setup (Ubuntu 20.04)
### Install pip
```bash 
sudo apt install python3-pip
```
### Install ROS Noetic
Taken from [here](http://wiki.ros.org/noetic/Installation/Ubuntu) (date accessed 23/05/2023)

#### Setup your sources.list
```
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
```
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
## Install yolov7-ros
### Download source from github
```bash
git clone https://github.com/jacob-m-nash/yolov7-ros.git ~/catkin_workspace/src/
```
### Install requirements
```bash
pip install -r ~/catkin_workspace/src/yolov7-ros/requirements.txt
```
```bash
sudo apt install ros-noetic-vision-msgs
``` 
```bash
sudo apt install ros-noetic-geometry-msgs

```
### Download test weights (TODO: Use fire detector training weights)
```bash
wget https://github.com/WongKinYiu/yolov7/releases/download/v0.1/yolov7.pt -O /path/to/dir/yolo7.pt
```
```bash
wget https://github.com/amikelive/coco-labels/blob/master/coco-labels-2014_2017.txt -O /path/to/dir/coco.txt
```
### Test build
```bash
cd ~/catkin_workspace
```
```bash
catkin build yolov7_ros
```
### Download Other Dependencies
usb-cam (for testing)
```bash
sudo apt install ros-noetic-usb-cam
``` 
## Build everything else
```
catkin build
```


## Launch
```ROS
roslaunch fire_detector fire_detector.launch 
```

## Simulation


## Download CHAMP
```bash
cd ~/catkin_workspace/src
```
```bash
git clone https://github.com/chvmp/champ.git
```

### C++ version fix
CHAMP may not build first time. To fix edit:
```
 champ/champ_gazebo/CMakeLists.txt
 ```
 and change the line 
 ```
 add_compile_options(-std=c++11)
 ```
 to 
 ```
 add_compile_options(-std=c++17)
 ```

## Download Spot Description

```bash
cd ~/catkin_workspace/src
```
```bash
git clone https://github.com/chvmp/robots.git
```
### Download URDF files
 ```bash
cd ~/catkin_workspace/src/src/robots
```
```bash
./install_descriptions
```

## Launch
### Launch gazebo simulation
```bash
roslaunch spot_config gazebo.launch 
```

### Launch robot teleop
```bash
roslaunch champ_teleop teleop.launch
```
TODO: nested packages dont work. Need to fix 