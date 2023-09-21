#!/usr/bin/env python
import yaml
import rospy
import np
import cv2
from fire_detector.srv import ObjectMapper, ObjectMapperResponse

font = cv2.FONT_HERSHEY_DUPLEX
font_size = 1
text_offset = 10,10
red = (0,0,255)

def map_objects(req):


    initial_state = np.array([0, 0])
    state_estimate = initial_state
    state_covariance = np.eye(2)
    process_noise = np.eye(2) * 0.1
    observation_noise = np.eye(2) * 1
    observation_matrix = np.eye(2)
    state_transition_matrix = np.eye(2)

    # Kalman filter loop for 2D state
    for locs in req.object_location:
        x = locs.pose.position.x
        y = locs.pose.position.y
        observation = np.array([x, y])
        # Prediction step
        predicted_state = np.dot(state_transition_matrix, state_estimate)
        predicted_covariance = np.dot(np.dot(state_transition_matrix, state_covariance), state_transition_matrix.T) + process_noise
        
        # Update step
        innovation = observation - np.dot(observation_matrix, predicted_state)
        innovation_covariance = np.dot(np.dot(observation_matrix, predicted_covariance), observation_matrix.T) + observation_noise
        kalman_gain = np.dot(np.dot(predicted_covariance, observation_matrix.T), np.linalg.inv(innovation_covariance))
        state_estimate = predicted_state + np.dot(kalman_gain, innovation)
        state_covariance = predicted_covariance - np.dot(np.dot(kalman_gain, observation_matrix), predicted_covariance)





    imagePath = "/home/jacob/catkin_workspace/src/Fire-Detector-ROS-Node/maps/marked_map.png" #TODO if png exists create new one, else load 
    map_file = req.map_path
    yaml_file = map_file.replace("pgm", "yaml")
    with open(yaml_file, "r") as stream:
        map_settings = yaml.safe_load(stream)
    map_origin_offset = map_settings["origin"]
    map = cv2.imread(map_file)



    map_coords = (state_estimate[0],state_estimate[1])
    object_image_center = map_to_img_coords(map_coords,map.shape,map_origin_offset)

    cv2.circle(map,object_image_center, 5, red, -1)
    
    text_loc = (object_image_center[0] + text_offset[0], object_image_center[1] + text_offset[1])
    cv2.putText(map,'fire_extinguisher',text_loc, font, font_size,red,2,cv2.LINE_AA)
    
    cv2.imwrite(imagePath,map)

    return ObjectMapperResponse(imagePath,True)

def map_to_img_coords(map_coords,map_size,map_origin_offset):
    map_x = map_coords[0]
    map_y = map_coords[1]
    map_height = map_size[0]
    map_offset_x, map_offset_y,_ = map_origin_offset
    
    x = map_x + map_offset_x

    y = map_height - map_y - map_offset_y

    return(x,y)


def fire_detector_mapper_server():
    rospy.init_node('fire_detector_mapper_server')
    s = rospy.Service('ObjectMapper', ObjectMapper, map_objects)
    rospy.spin()

if __name__ == "__main__":
    fire_detector_mapper_server()
