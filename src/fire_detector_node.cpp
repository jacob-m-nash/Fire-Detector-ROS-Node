
#include <fire_detector.h>
#include <fire_detector_nav.h>
#include <tf/transform_broadcaster.h>
#include <tf2/LinearMath/Quaternion.h>
#include <math.h>

int main(int argc, char **argv){
  
  ros::init(argc,argv, "fire_detector node");
  ros::NodeHandle nh;

  // create an instance of the fire_detector node
  fire_detector fire_detector(nh);

  // add camera frame
  tf::TransformBroadcaster br;
  tf::Transform transform;

  //fire_detector_nav fire_detector_nav(nh);

  // loop rate in Hz
  ros::Rate rate(60);

  while (ros::ok()) {

    transform.setOrigin( tf::Vector3(0.0, 2.0, 0.0) );

    static tf2::Quaternion myQuaternion;
    myQuaternion.setRPY(0, 0, 0.25 * M_PI);
    transform.setRotation( tf::Quaternion(myQuaternion.getX(), myQuaternion.getY(), myQuaternion.getZ(), myQuaternion.getW()) );
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "body", "camera"));

    //fire_detector_nav.navToPoint(10,10);
    // spin and process all pending callbacks
    ros::spinOnce();

    // sleep to fulfill the loop rate
    rate.sleep();
  }
}