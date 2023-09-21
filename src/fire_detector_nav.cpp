#include "fire_detector_nav.h"

FireDetectorNavAction::FireDetectorNavAction(std::string name)
: as_(nh_, name, boost::bind(&FireDetectorNavAction::executeCB, this, _1), false),
  action_name_(name),
  move_base_client_("move_base", true) {
  as_.start();
}

void FireDetectorNavAction::executeCB(const your_package_name::FireDetectorNavGoalConstPtr &goal) {
  bool success = true;
  move_base_msgs::MoveBaseGoal move_base_goal;

  for (size_t i = 0; i < goal->pose_list.size(); ++i) {
    if (as_.isPreemptRequested() || !ros::ok()) {
      as_.setPreempted();
      success = false;
      break;
    }
    
    move_base_goal.target_pose = goal->pose_list[i];
    move_base_client_.sendGoal(move_base_goal);
    move_base_client_.waitForResult();
    
    if (move_base_client_.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
      feedback_.current_index = i;
      as_.publishFeedback(feedback_);
    } else {
      success = false;
      break;
    }
  }

  if (success) {
    result_.status = "All goals reached successfully";
    as_.setSucceeded(result_);
  } else {
    result_.status = "Failed to reach one or more goals";
    as_.setAborted(result_);
  }
}

