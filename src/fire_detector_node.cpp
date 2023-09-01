
#include <fire_detector.h>
#include <fire_detector_nav.h>

int main(int argc, char **argv){
  
  ros::init(argc,argv, "fire_detector node");
  ros::NodeHandle nh;

  // create an instance of the fire_detector node
  fire_detector fire_detector(nh);

  //fire_detector_nav fire_detector_nav(nh);

  // loop rate in Hz
  ros::Rate rate(60);

  while (ros::ok()) {

    //fire_detector_nav.navToPoint(10,10);
    // spin and process all pending callbacks
    ros::spinOnce();

    // sleep to fulfill the loop rate
    rate.sleep();
  }
}