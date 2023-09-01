#include <fire_detector.h>

fire_detector::fire_detector(ros::NodeHandle nh){
    nh_ = nh;
    // Initialise ROS Subscribers
    cam_sub_ = nh_.subscribe("/usb_cam/image_raw",1,&fire_detector::cameraCallback, this);
    detect_sub_ = nh_.subscribe("/yolov7/yolov7/visualization",1,&fire_detector::detectionCallback,this);
    detect_sub_2 = nh_.subscribe("/yolov5/detections", 1, &fire_detector::boundingBoxCallback,this);

    // Define the publishers
    img_pub_ = nh_.advertise<sensor_msgs::Image>("/camera/rgb/image_raw", 1, true);

    last_time_ = std::chrono::steady_clock::now();
    current_time_ = std::chrono::steady_clock::now();
}

void fire_detector::cameraCallback(const sensor_msgs::Image::ConstPtr& msg)
{
  img_pub_.publish(msg);
}



void fire_detector::detectionCallback(const sensor_msgs::Image::ConstPtr &msg){
    current_time_ = std::chrono::steady_clock::now();
    float htz = calculateFrequency(last_time_,current_time_);
    ROS_INFO("htz %f", htz);
    last_time_ = std::chrono::steady_clock::now();
    return;

}

void fire_detector::boundingBoxCallback(const darknet_ros_msgs::BoundingBoxes::ConstPtr &msg)
{
    for (darknet_ros_msgs::BoundingBox box : msg->bounding_boxes){
        std::string objClass = box.Class;
        if(objClass == "Fire_Extinguisher" ){
            float d = box.depth;
            ROS_INFO("Depth: %f", d);
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
