#ifndef FIRE_DETECTOR_H
#define FIRE_DETECTOR_H

#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include "geometry_msgs/PoseStamped.h"
#include "nav_msgs/Odometry.h"
#include <ctime>
#include <chrono>
#include "detection_msgs/BoundingBoxes.h"


class fire_detector{
    public:
        fire_detector(ros::NodeHandle nh);

        ros::NodeHandle nh_;
        ros::Subscriber detect_sub_;
        ros::Subscriber pose_sub_;
        std::chrono::steady_clock::time_point current_time_;
        std::chrono::steady_clock::time_point last_time_;
        ros::Publisher img_pub_;
        geometry_msgs::Pose spotPose;   

        void spotOdoCallback(const nav_msgs::Odometry::ConstPtr &msg);
    
        void detectionCallback(const sensor_msgs::Image::ConstPtr &msg);

        void boundingBoxCallback(const detection_msgs::BoundingBoxes::ConstPtr &msg);

        float calculateFrequency(std::chrono::steady_clock::time_point previous_time, std::chrono::steady_clock::time_point current_time);
        void matrixVectorMultiply(const double matrix[3][4], const double vector[3], double result[3]);
        
};

#endif