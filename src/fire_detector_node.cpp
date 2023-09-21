#include <fire_detector_projector.h>
#include <fire_detector_mapper.h>
#include <actionlib/client/simple_action_client.h>
#include <fire_detector_nav.h>
#include <math.h>
#include <list>
#include <geometry_msgs/PoseStamped.h>

int main(int argc, char **argv){
  
  ros::init(argc,argv, "fire_detector node");
  ros::NodeHandle nh;

  // create an instance of the fire_detector_projection node
  fire_detector_projector fire_detector_projector(nh);


  actionlib::SimpleActionClient<fire_detector::FireDetectorNavAction> fire_detector_nav_client("fire_detector_move_base", true);
  fire_detector_nav_client.waitForServer();

  fire_detector::FireDetectorNavGoal goal;


  geometry_msgs::PoseStamped pose;


  pose.header.frame_id = "map";
  pose.header.stamp = ros::Time::now();

  pose.pose.position.x = 400.0;
  pose.pose.position.y = 0.0;
  pose.pose.position.z = 0.0; 
  


  goal.pose_list.push_back(pose);


  fire_detector_nav_client.sendGoal(goal);

 
  while (ros::ok()) {
    ros::spinOnce();
    if (fire_detector_nav_client.getState().isDone()) {
      if (fire_detector_nav_client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
        ROS_INFO("Successfully reached all goals.");
        break;
      } else {
        ROS_ERROR("Failed to reach one or more goals.");
        break;
      }
    } else {
      ROS_INFO("Still working on reaching goals...");
    }
    rate.sleep();
  }

  // mapping 
   ros::ServiceClient client = nh.serviceClient<fire_detector::ObjectMapper>("object_mapper_service");


  fire_detector::ObjectMapper srv;

  srv.request.object_locations = fire_detector_projector.object_map_points_;
  
  srv.request.map_path = "Fire-Detector-ROS-Node/maps/RPL_lab_test_area.pgm";

  // Call the service
  if (client.call(srv))
  {
    if (srv.response.success)
    {
      ROS_INFO("Map made.");
    }
    else
    {
      ROS_ERROR("Mapping failed.");
    }
  }
  else
  {
    ROS_ERROR("Failed to call map service.");
  }

  while (ros::ok()) {

    ros::spinOnce();

    // sleep to fulfill the loop rate
    rate.sleep();
  }
}