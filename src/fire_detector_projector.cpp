#include <fire_detector_projector.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/PoseStamped.h>
#include <list>
#include "fire_detector/ObjectMapper.h"
#include <fstream>


fire_detector_projector::fire_detector_projector(ros::NodeHandle nh){
    nh_ = nh;
    // Initialise ROS Subscribers
    detect_sub_ = nh_.subscribe("/yolov5/detections", 1, &fire_detector_projector::boundingBoxCallback,this);
    
    std::list<geometry_msgs::PoseStamped> object_map_points_ = {};
    tf::TransformListener listener_;
  

}


void fire_detector_projector::boundingBoxCallback(const detection_msgs::BoundingBoxes::ConstPtr &msg)
{
    
    std::ofstream my_file;
    my_file.open("observations.csv", std::ios::out | std::ios::app);
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
            double k [3][3] = {{922.5476684570312, 0.0, 653.406982421875}, {0.0, 922.5319213867188, 360.19708251953125}, {0.0, 0.0, 1.0}};
            float fx = k[0][0];
            float fy = k[1][1];
            float cx = k[0][2];
            float cy = k[1][2];

            float projected_z = d ; 
            float projected_x = ((x - cx) * d) / fx; 
            float projected_y = ((y - cy) * d) / fy;
            ROS_INFO("x %f y %f z %f", projected_x,projected_y, projected_z);

            geometry_msgs::PoseStamped camera_pose;
            camera_pose.header.frame_id = "camera";
         

            camera_pose.header.stamp = ros::Time();
         
            
            camera_pose.pose.position.x = projected_x;
            camera_pose.pose.position.y = projected_y;
            camera_pose.pose.position.z = projected_z;
            camera_pose.pose.orientation.x = 0;
            camera_pose.pose.orientation.y = 0;
            camera_pose.pose.orientation.z = 0;
            camera_pose.pose.orientation.w = 1;
    
            geometry_msgs::PoseStamped map_pose;
            fire_detector_projector::listener_.transformPose("/map", camera_pose, map_pose);


            ROS_INFO("New x %f y %f z %f", map_pose.pose.position.x  ,map_pose.pose.position.y, map_pose.pose.position.z);
            my_file << map_pose.pose.position.x << "," << map_pose.pose.position.y << "," << box.probability << std::endl;
            object_map_points_.push_back(map_pose);
        }
        my_file.close();
        
    }

   
    
}


