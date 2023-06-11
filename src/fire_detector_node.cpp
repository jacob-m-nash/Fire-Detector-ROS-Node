
#include <fire_detector.h>

int main(int argc, char **argv){
  
  ros::init(argc,argv, "fire_detector node");
  ros::NodeHandle nh;

  // create an instance of the cw3 class
  fire_detector fire_detector(nh);

  // loop rate in Hz
  ros::Rate rate(60);

  while (ros::ok()) {

    // spin and process all pending callbacks
    ros::spinOnce();

    // sleep to fulfill the loop rate
    rate.sleep();
  }
}