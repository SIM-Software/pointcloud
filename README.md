# pointcloud

## Photos Info
The only photos I currently have are rendered in Blender. These photos are from a simple environment where 63 lines are being projected onto the objects.

Eventually, I plan on adding more sets of photos, specifically ones with rotated lines, and new environments like the moon's surface.

## General Info  
Scripts are located at ```pointcloud/src```

I plan on using ROS2 Humble with Rviz2 to visualize the pointcloud. Currently, there are no scripts using ROS, but keep this in mind for later scripts.

## Version Info
g++ version: 11.4.0  
OpenCV Version: 4.5.4 

This is not a requirement, this is just what I'm running the scripts on

## Running the Script
```g++ -Wall -o main main.cpp `pkg-config --cflags --libs opencv4` ```  

```./main```
