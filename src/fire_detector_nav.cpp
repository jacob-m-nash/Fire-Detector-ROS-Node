#include "fire_detector_nav.h"
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

fire_detector_nav::fire_detector_nav(ros::NodeHandle nh)
{
    nh_ = nh;    
}

void fire_detector_nav::navToPoint(float x, float y)
{
    //TODO:  move base server name 
    //TODO:  launch move base from fire node
    MoveBaseClient moveBaseClient_("move_base", true);
    while (!moveBaseClient_.waitForServer(ros::Duration(5.0)))
    {
        ROS_INFO("Waiting for move_base server to start");
    }
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "base_link"; //TODO is this the right link?
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x = x;
    goal.target_pose.pose.position.y = 1.0;

    moveBaseClient_.sendGoal(goal);
    moveBaseClient_.waitForResult();

}
