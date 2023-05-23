#ifndef FIRE_DETECTOR_H
#define FIRE_DETECTOR_H

#include <ros/ros.h>
#include <sensor_msgs/Image.h>

class fire_detector{
    public:
        fire_detector(ros::NodeHandle nh);

        ros::NodeHandle nh_;
        ros::Subscriber cam_sub_;
        ros::Subscriber detect_sub_;

        ros::Publisher img_pub_;

        void   
        cameraCallback(const sensor_msgs::Image& msg);

        void   
        detectionCallback(const sensor_msgs::Image& msg);

};

#endif