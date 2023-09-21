#ifndef FIRE_DETECTOR_H
#define FIRE_DETECTOR_H

#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include "detection_msgs/BoundingBoxes.h"
#include <tf/transform_listener.h>
#include <geometry_msgs/Pose.h>
#include <fire_detector_projector.h>
#include <list>

class fire_detector_projector{
    public:
        fire_detector_projector(ros::NodeHandle nh);

        ros::NodeHandle nh_;
        ros::Subscriber detect_sub_;

        std::list<geometry_msgs::PoseStamped> object_map_points_;
    
        void detectionCallback(const sensor_msgs::Image::ConstPtr &msg);

        void boundingBoxCallback(const detection_msgs::BoundingBoxes::ConstPtr &msg);

        tf::TransformListener listener_;

    
        
};

#endif