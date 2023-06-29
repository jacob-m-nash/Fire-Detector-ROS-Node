#include <fire_detector.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <stdlib.h>
#include <boost/foreach.hpp>

fire_detector::fire_detector(ros::NodeHandle nh){
    nh_ = nh;
    // Initialise ROS Subscribers
    cam_sub_ = nh_.subscribe("/usb_cam/image_raw",1,&fire_detector::cameraCallback, this);
    detect_sub_ = nh_.subscribe("/yolov7/yolov7/visualization",1,&fire_detector::detectionCallback,this);

    // Define the publishers
    img_pub_ = nh_.advertise<sensor_msgs::Image>("/camera/rgb/image_raw", 1, true);

    bag_pub_ = nh.advertise<sensor_msgs::Image>("/video/image",1,true);

    last_time_ = std::chrono::steady_clock::now();
    current_time_ = std::chrono::steady_clock::now();

    std::string bag_path;
    nh_.getParam("/fire_detector_node/bag_path", bag_path);
    //openBag(bag_path);
}

void fire_detector::cameraCallback(const sensor_msgs::Image::ConstPtr& msg)
{
  img_pub_.publish(msg);
}


void fire_detector::detectionCallback(const sensor_msgs::Image::ConstPtr &msg){
    current_time_ = std::chrono::steady_clock::now();
    float htz = calculateFrequency(last_time_,current_time_);
    ROS_INFO("htz %f", htz);
    last_time_ = std::chrono::steady_clock::now();
    return;

}

float fire_detector::calculateFrequency(std::chrono::steady_clock::time_point previous_time, std::chrono::steady_clock::time_point current_time)
{
    std::chrono::steady_clock::duration time_span = current_time - previous_time;
    double nseconds = double(time_span.count()) * std::chrono::steady_clock::steady_clock::period::num / std::chrono::steady_clock::steady_clock::period::den;
    float htz = 1/nseconds;
    return htz;
}

void fire_detector::openBag(std::string filePath){
    rosbag::Bag bag;
    bag.open(filePath);
    rosbag::View view(bag,rosbag::TopicQuery("/device_0/sensor_1/Color_0/image/data"));
    ros::Time bag_begin_time = view.getBeginTime();
    ros::Time bag_end_time = view.getEndTime();
    double len = (bag_end_time-bag_begin_time).toSec();
    ROS_INFO("bag length: %f",len);
    sleep(10);
    ROS_INFO("starting reading messages");
    int count = 0;
    BOOST_FOREACH(rosbag::MessageInstance const m, view){
        sensor_msgs::Image::ConstPtr i = m.instantiate<sensor_msgs::Image>();
        if (i != nullptr){
            bag_pub_.publish(m);
            count ++;
        }
    }
    ROS_INFO("number of msg %i", count);
}
