# pointcloud

## General Info  
Scripts are located at ```pointcloud/src```

I plan on using ROS2 Humble with Rviz2 to visualize the pointcloud. Currently, there are no scripts using ROS, but keep this in mind for later scripts.

## Photos Info
The only photos I currently have are rendered in Blender. These photos are from a simple environment where 63 lines are being projected onto the objects.

Eventually, I plan on adding more sets of photos, specifically ones with rotated lines, and new environments like the moon's surface.

## Version Info
g++ version: 11.4.0  
OpenCV Version: 4.5.4 

This is not a requirement, this is just what I'm running the scripts on

## Python Scripts
The first iteration of this was coded in python 3.10.12, but it is very slow.

```generate_masks.py``` Puts a mask over the images to make all the red pixels white, and everything else black. I plan to move this to C++ as well at some point.

```main.py``` Similar to main.cpp, but in python. Takes around 30 seconds to run, but you can visualize the pointcloud in Rviz once finished. ROS2 Humble needed for this. (Use Linux, not Windows).

## Running the Main Script
```g++ -Wall -o main main.cpp `pkg-config --cflags --libs opencv4` ```  

```./main```
