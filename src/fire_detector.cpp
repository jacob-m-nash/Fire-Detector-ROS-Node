#include <fire_detector.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include<array>//todo maybe remove??
#include <geometry_msgs/PoseStamped.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

#include <tf2_ros/transform_listener.h>

fire_detector::fire_detector(ros::NodeHandle nh){
    nh_ = nh;
    // Initialise ROS Subscribers
    detect_sub_ = nh_.subscribe("/yolov5/detections", 1, &fire_detector::boundingBoxCallback,this);
    pose_sub_ = nh_.subscribe("/spot/odometry", 1, &fire_detector::spotOdoCallback,this);

  
    
    last_time_ = std::chrono::steady_clock::now();
    current_time_ = std::chrono::steady_clock::now();

}

void fire_detector::spotOdoCallback(const nav_msgs::Odometry::ConstPtr &msg)
{
    fire_detector::spotPose = msg->pose.pose;
}


void fire_detector::detectionCallback(const sensor_msgs::Image::ConstPtr &msg)
{
    current_time_ = std::chrono::steady_clock::now();
    float htz = calculateFrequency(last_time_,current_time_);
    ROS_INFO("htz %f", htz);
    last_time_ = std::chrono::steady_clock::now();
    return;
}

void fire_detector::boundingBoxCallback(const detection_msgs::BoundingBoxes::ConstPtr &msg)
{
    for (detection_msgs::BoundingBox box : msg->bounding_boxes){
        std::string objClass = box.Class;
        if(objClass == "Fire_Extinguisher" ){
            double d = box.depth;
            double x = (double)box.centerX;
            double y =(double) box.centerY;
            ROS_INFO("detect: %s", objClass.c_str());
            double result[3];
            double camera_matrix[3][4] = { //TODO: remove hard coded P matrix 
                {922.5476684570312, 0.0, 653.406982421875, 0.0},
                { 0.0, 922.5319213867188, 360.19708251953125, 0.0},
                {0.0, 0.0, 1.0, 0.0}
            };
            float inv_fx = 1.f/camera_matrix[0][0];
            float inv_fy = 1.f/camera_matrix[1][1];
            float cx = camera_matrix[0][2];
            float cy = camera_matrix[1][2];

            float realworld_x = d; 
            float realworld_y = -(x - cx) * d * inv_fx; 
            float realworld_z = -(y - cy) * d * inv_fy;
            ROS_INFO("x %f y %f z %f", realworld_x,realworld_y, realworld_z);
            geometry_msgs::PoseStamped output_msg;
            output_msg.pose.position.x = realworld_x;
            output_msg.pose.position.y = realworld_y;
            output_msg.pose.position.z = realworld_z;
            tf::StampedTransform transform;
            tf::TransformListener listener;
            listener.lookupTransform("camera", "body",ros::Time(0), transform);
            //base_link_to_leap_motion = tf_buffer.lookupTransform("camera", "body", ros::Time::now(), ros::Duration(1.0) );
            //tf2::doTransform(output_msg.pose, output_msg.pose, base_link_to_leap_motion);
        }
        
    }
    
}

float fire_detector::calculateFrequency(std::chrono::steady_clock::time_point previous_time, std::chrono::steady_clock::time_point current_time)
{
    std::chrono::steady_clock::duration time_span = current_time - previous_time;
    double nseconds = double(time_span.count()) * std::chrono::steady_clock::steady_clock::period::num / std::chrono::steady_clock::steady_clock::period::den;
    float htz = 1/nseconds;
    return htz;
}

