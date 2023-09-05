#ifndef FIRE_DETECTOR_TF_H
#define FIRE_DETECTOR_TF_H

#include <ros/ros.h>
#include <tf/transform_listener.h>

class fire_detector_tf{
    public:
        fire_detector_tf(ros::NodeHandle nh);

        ros::NodeHandle nh_;
     
        tf::TransformListener listener;
        
};

#endif