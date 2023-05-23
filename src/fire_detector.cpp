#include <fire_detector.h>

fire_detector::fire_detector(ros::NodeHandle nh){
    nh_ = nh;
    // Initialise ROS Subscribers
    cam_sub_ = nh.subscribe("/usb_cam/image_raw",1,&fire_detector::cameraCallback, this);

    // Define the publishers
}

void fire_detector::cameraCallback(const sensor_msgs::Image &msg)
{
}
