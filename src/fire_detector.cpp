#include <fire_detector.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <stdlib.h>
#include <boost/foreach.hpp>
#include <rosbag/bag_player.h>

fire_detector::fire_detector(ros::NodeHandle nh){
    nh_ = nh;
    // Initialise ROS Subscribers
    cam_sub_ = nh_.subscribe("/usb_cam/image_raw",1,&fire_detector::cameraCallback, this);
    detect_sub_ = nh_.subscribe("/yolov7/yolov7/visualization",1,&fire_detector::detectionCallback,this);
    yolo_state_sub_ = nh_.subscribe("/yolov7/yolov7/status",1,&fire_detector::yolo_status_callback, this);

    // Define the publishers
    img_pub_ = nh_.advertise<sensor_msgs::Image>("/camera/rgb/image_raw", 1, true);

    bag_pub_ = nh.advertise<sensor_msgs::Image>("/video/image",1,true);

    last_time_ = std::chrono::steady_clock::now();
    current_time_ = std::chrono::steady_clock::now();
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

void fire_detector::yolo_status_callback(const std_msgs::String::ConstPtr &msg){
    std::string status = msg->data.c_str();
    if(status == "ready"){
        std::string bag_path;
        nh_.getParam("/fire_detector_node/bag_path", bag_path);
        openBag(bag_path);
    }
}

float fire_detector::calculateFrequency(std::chrono::steady_clock::time_point previous_time, std::chrono::steady_clock::time_point current_time)
{
    std::chrono::steady_clock::duration time_span = current_time - previous_time;
    double nseconds = double(time_span.count()) * std::chrono::steady_clock::steady_clock::period::num / std::chrono::steady_clock::steady_clock::period::den;
    float htz = 1/nseconds;
    return htz;
}

void fire_detector::openBag(std::string filePath){
    rosbag::Bag bag (filePath);
    rosbag::View view(bag,rosbag::TopicQuery("/device_0/sensor_1/Color_0/image/data"));
    ros::Time prev_time = view.getBeginTime();
    ROS_INFO("starting reading messages");
    BOOST_FOREACH(rosbag::MessageInstance const m, view){
        ros::Time message_time =  m.getTime();
        (message_time - prev_time).sleep();
        sensor_msgs::Image::ConstPtr i = m.instantiate<sensor_msgs::Image>();
        if (i != nullptr){
            bag_pub_.publish(m);
        }
        prev_time = message_time;
    }

}


