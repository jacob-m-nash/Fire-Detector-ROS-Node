#include <tf/transform_broadcaster.h>
#include <tf2/LinearMath/Quaternion.h>
#include <math.h>

int main(int argc, char **argv){
  
  ros::init(argc,argv, "fire_detector_tf node");

  

  // add camera frame
  tf::TransformBroadcaster br;
  tf::Transform transform;

  transform.setOrigin( tf::Vector3(0.120, -0.080, 0.125) );

  static tf2::Quaternion myQuaternion;
  myQuaternion.setRPY(0, 0, - 0.5 * M_PI);
  transform.setRotation( tf::Quaternion(myQuaternion.getX(), myQuaternion.getY(), myQuaternion.getZ(), myQuaternion.getW()) );
  

  // loop rate in Hz
  ros::Rate rate(60);

  while (ros::ok()) {
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "body", "camera"));



    //fire_detector_nav.navToPoint(10,10);
    // spin and process all pending callbacks
    ros::spinOnce();

    // sleep to fulfill the loop rate
    rate.sleep();
  }
}