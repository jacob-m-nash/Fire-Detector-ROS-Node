#include <fire_detector.h>

fire_detector::fire_detector(ros::NodeHandle nh){
    nh_ = nh;
    // Initialise ROS Subscribers
    cam_sub_ = nh_.subscribe("/usb_cam/image_raw",1,&fire_detector::cameraCallback, this);
    detect_sub_ = nh_.subscribe("darknet_ros/detection_image",1,&fire_detector::detectionCallback,this);

    // Define the publishers
    img_pub_ = nh_.advertise<sensor_msgs::Image>("/camera/rgb/image_raw", 1, true);
}

void fire_detector::cameraCallback(const sensor_msgs::Image::ConstPtr& msg)
{
  ROS_INFO("Received image with size: %i x %i", msg->width, msg->height);
  img_pub_.publish(msg);
}



void fire_detector::detectionCallback(const sensor_msgs::Image::ConstPtr &msg){
    ROS_INFO("Object Found");
    return;

}
