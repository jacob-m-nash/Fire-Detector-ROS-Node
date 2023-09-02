#include <fire_detector.h>
#include <tf/transform_broadcaster.h>
#include<array> //maybe std::

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
            double matrix[3][4] = { //TODO: remove hard coded P matrix 
                {922.5476684570312, 0.0, 653.406982421875, 0.0},
                { 0.0, 922.5319213867188, 360.19708251953125, 0.0},
                {0.0, 0.0, 1.0, 0.0}
            };
            double vector[3] = {x, y, d};
            matrixVectorMultiply(matrix, vector, result);
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

void fire_detector::matrixVectorMultiply(const double matrix[3][4], const double vector[3], double result[3]) {
    for (int i = 0; i < 3; ++i) {
        result[i] = 0.0;
        for (int j = 0; j < 4; ++j) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
}
