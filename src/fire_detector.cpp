#include <fire_detector.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include<array>//todo maybe remove??
#include <geometry_msgs/PoseStamped.h>
//#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

//#include <tf2_ros/transform_listener.h>

fire_detector::fire_detector(ros::NodeHandle nh){
    nh_ = nh;
    // Initialise ROS Subscribers
    detect_sub_ = nh_.subscribe("/yolov5/detections", 1, &fire_detector::boundingBoxCallback,this);
    pose_sub_ = nh_.subscribe("/spot/odometry", 1, &fire_detector::spotOdoCallback,this);

  
    
    last_time_ = std::chrono::steady_clock::now();
    current_time_ = std::chrono::steady_clock::now();
    

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

            float projected_x = d; 
            float projected_y = -(x - cx) * d * inv_fx; 
            float projected_z = -(y - cy) * d * inv_fy;
            ROS_INFO("x %f y %f z %f", projected_x,projected_y, projected_z);

            geometry_msgs::PointStamped camera_point;
            camera_point.header.frame_id = "camera";
         

            camera_point.header.stamp = ros::Time();
         
            
            camera_point.point.x = projected_x;
            camera_point.point.y = projected_y;
            camera_point.point.z = projected_z;
    
            geometry_msgs::PointStamped map_point;
            listener.transformPoint("map", laser_point, map_point);

            ROS_INFO("New x %f y %f z %f", map_point.point.x  ,map_point.point.y, map_point.point.z);
        }
        
    }
    
}


