import cv2

import os.path
 

prompt = '> '
 
print("What is the name of your floor plan you want to convert to a ROS map:") 
file_name = input(prompt)
image = cv2.imread(file_name)
res = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)


print("What is the resolution of the image (pixel/m)?")
resolution = 1/float(input(prompt)) # we need meters/pixel

print("What are the origin coordinates of the map?")
xOrigin = float(input("x:"))
yOrigin = float(input("y:"))
wOrigin = float(input("w (default: 0):"))

print("What is the name of the new map?")
mapName = input(prompt)
mapFileName = mapName + ".pgm"
cv2.imwrite(mapFileName, res );

yamlFileName = mapName + ".yaml"
with open(yamlFileName, "w") as yaml:
    yaml.write(f"image: {mapFileName}\n")
    yaml.write(f"resolution: {resolution}\n")
    yaml.write(f"origin: [ {xOrigin} , {yOrigin}  ,  {wOrigin}]\n")
    yaml.write("negate: 0\noccupied_thresh: 0.65\nfree_thresh: 0.196")
    

