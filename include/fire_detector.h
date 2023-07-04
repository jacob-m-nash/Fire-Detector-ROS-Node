#ifndef FIRE_DETECTOR_H
#define FIRE_DETECTOR_H

#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/String.h>
#include <ctime>
#include <chrono>
#include <rosbag/bag.h>
 #include <rosbag/view.h>
 #include <boost/foreach.hpp>

class fire_detector{
    public:
        fire_detector(ros::NodeHandle nh);

        ros::NodeHandle nh_;
        ros::Subscriber cam_sub_;
        ros::Subscriber detect_sub_;
        ros::Subscriber yolo_state_sub_;
        std::chrono::steady_clock::time_point current_time_;
        std::chrono::steady_clock::time_point last_time_;
        ros::Publisher img_pub_;
        ros::Publisher bag_pub_;


        void cameraCallback(const sensor_msgs::Image::ConstPtr &msg);

        void detectionCallback(const sensor_msgs::Image::ConstPtr &msg);

        void yolo_status_callback(const std_msgs::String::ConstPtr &msg);

        float calculateFrequency(std::chrono::steady_clock::time_point previous_time, std::chrono::steady_clock::time_point current_time);

        void openBag(std::string filePath);
        
};

#endif