#include <fire_detector.h>
#include <fire_detector_nav.h>
#include <math.h>

int main(int argc, char **argv){
  
  ros::init(argc,argv, "fire_detector_tf node");
  ros::NodeHandle nh;

  // create an instance of the fire_detector node
  fire_detector fire_detector(nh);
  

  // add camera frame
  // tf::TransformBroadcaster br;
  // tf::Transform transform;

  fire_detector_nav fire_detector_nav(nh);

  // loop rate in Hz
  ros::Rate rate(60);

  while (ros::ok()) {

    ros::spinOnce();

    // sleep to fulfill the loop rate
    rate.sleep();
  }
}