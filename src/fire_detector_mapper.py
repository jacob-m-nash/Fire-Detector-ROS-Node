#!/usr/bin/env python
import yaml
import rospy
import cv2
from fire_detector.srv import ObjectMapper, ObjectMapperResponse

font = cv2.FONT_HERSHEY_DUPLEX
font_size = 2
text_offset = 10,10
red = (0,0,255)

def map_objects(req):
    imagePath = "/home/jacob/catkin_workspace/src/Fire-Detector-ROS-Node/maps/marked_map.png" #TODO change name
    map_file = req.map_path
    yaml_file = map_file.replace("pgm", "yaml")
    map_settings = yaml.load(yaml_file)
    map_origin_offset = map_settings["origin"]
    map = cv2.imread(map_file)
    for object_loc in req.object_locations:
        map_coords = (object_loc.position.x,object_loc.position.y)
        object_image_center = map_to_img_coords(map_coords,map.shape,map_origin_offset)

        cv2.circle(map,object_image_center, 5, red, -1)
        
        if(object_image_center[0] > map.shape[0]):
            text_loc = (object_image_center[0] - text_offset[0], object_image_center[1] + text_offset[1])
            cv2.putText(map,'fire_extinguisher',text_loc, font, font_size,red,2,cv2.LINE_AA,bottomLeftOrigin=True)
        else:
            text_loc = tuple(map(lambda i, j: i + j, object_image_center, text_offset))
            cv2.putText(map,'fire_extinguisher',text_loc, font, font_size,red,2,cv2.LINE_AA,bottomLeftOrigin=False)
    
    cv2.imwrite(imagePath,map)

    return ObjectMapperResponse(imagePath,True)

def map_to_img_coords(map_coords,map_size,map_origin_offset):
    map_x = map_coords[0]
    map_y = map_coords[1]
    map_height = map_size[0]
    map_offset_x, map_offset_y = map_origin_offset
    
    x = map_x + map_offset_x

    y = map_height - map_y - map_offset_y

    return(x,y)


def fire_detector_mapper_server():
    rospy.init_node('fire_detector_mapper_server')
    s = rospy.Service('ObjectMapper', ObjectMapper, map_objects)
    rospy.spin()

if __name__ == "__main__":
    fire_detector_mapper_server()