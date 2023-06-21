#ifndef FIRE_DETECTOR_H
#define FIRE_DETECTOR_H

#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <ctime>
#include <chrono>

class fire_detector{
    public:
        fire_detector(ros::NodeHandle nh);

        ros::NodeHandle nh_;
        ros::Subscriber cam_sub_;
        ros::Subscriber detect_sub_;
        std::chrono::steady_clock::time_point current_time_;
        std::chrono::steady_clock::time_point last_time_;
        ros::Publisher img_pub_;


        void cameraCallback(const sensor_msgs::Image::ConstPtr &msg);

        void detectionCallback(const sensor_msgs::Image::ConstPtr &msg);

        float calculateFrequency(std::chrono::steady_clock::time_point previous_time, std::chrono::steady_clock::time_point current_time);
        
};

#endif