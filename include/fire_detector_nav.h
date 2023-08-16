#ifndef FIRE_DETECTOR_NAV_H
#define FIRE_DETECTOR_NAV_H

#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
class fire_detector_nav{
    public:
        fire_detector_nav(ros::NodeHandle nh);
        ros::NodeHandle nh_;
        void navToPoint(float x, float y);

};
#endif