#include <fire_detector.h>

fire_detector::fire_detector(ros::NodeHandle nh){
    nh_ = nh;
    // Initialise ROS Subscribers
    cam_sub_ = nh_.subscribe("/usb_cam/image_raw",1,&fire_detector::cameraCallback, this);
    detect_sub_ = nh_.subscribe("/yolov7/yolov7/visualization",1,&fire_detector::detectionCallback,this);

    // Define the publishers
    img_pub_ = nh_.advertise<sensor_msgs::Image>("/camera/rgb/image_raw", 1, true);
    last_time_ = std::chrono::steady_clock::now();
    current_time_ = std::chrono::steady_clock::now();
}

void fire_detector::cameraCallback(const sensor_msgs::Image::ConstPtr& msg)
{
  ROS_INFO("Received image with size: %i x %i", msg->width, msg->height);
  img_pub_.publish(msg);
}



void fire_detector::detectionCallback(const sensor_msgs::Image::ConstPtr &msg){
    ROS_INFO("Object Found");
    current_time_ = std::chrono::steady_clock::now();
    std::chrono::steady_clock::duration time_span = current_time_ - last_time_;
    double nseconds = double(time_span.count()) * std::chrono::steady_clock::steady_clock::period::num / std::chrono::steady_clock::steady_clock::period::den;
    float htz = 1/nseconds;
    ROS_INFO("htz %f", htz);
    last_time_ = std::chrono::steady_clock::now();
    return;

}


