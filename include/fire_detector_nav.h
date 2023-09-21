#ifndef MOVE_BASE_LIST_ACTION_SERVER_H_
#define MOVE_BASE_LIST_ACTION_SERVER_H_

#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <your_package_name/FireDetectorNavAction.h>  // Replace 'your_package_name' with the actual package name
#include <geometry_msgs/PoseStamped.h>

class FireDetectorNavAction {
protected:
  ros::NodeHandle nh_;
  actionlib::SimpleActionServer<your_package_name::FireDetectorNavAction> as_;
  std::string action_name_;
  your_package_name::MoveBaseListFeedback feedback_;
  your_package_name::MoveBaseListResult result_;
  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> move_base_client_;

public:
  FireDetectorNavAction(std::string name);
  void executeCB(const your_package_name::FireDetectorNavGoalConstPtr &goal);
};

#endif // MOVE_BASE_LIST_ACTION_SERVER_H_
