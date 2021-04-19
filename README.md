# AE740_FINAL_PROJECT
Final Project

## Installation

Tested on Ubuntu 18.04, using Ros Melodic.

Create a workspace:
```
~/$ cd
~/$ mkdir -p catkin_ws/src
```
Now clone the entire repo into the src folder:
```
~/$ cd catkin_ws/src/
~/catkin_ws/src$ git clone <repo url>
```
This should contain a copy of all/most of the dependencies we need, so hopefully we dont need to run extra installation processes. 

From the workspace folder, we just need to build the folder:
```
~/catkin_ws$ catkin build -j8
```

and now hopefully all the dependencies can be built.

In addition, you will need:

```
sudo apt-get install ros-melodic-octomap-rviz-plugins
```

and you might need to follow the installation instructions for OMPL, FCL, and Octomap (no octovis needed). Essentially you need to do the following:
```
cd dependencies/<package>
mkdir build
cd build
cmake ..
make -j8
sudo make install
```

If you have issues with `nlopt` not building, go into `catkin_ws/build/nlopt` and `sudo chmod +x make_install_nlopt.sh`

## Testing rotorS

using the basic 

```
roslaunch rotors_gazebo mav_hovering_example.launch mav_name:=firefly world_name:=basic
```

If you have issues with VMware, when launching gazebo:
```
export SVGA_VGPU10=0
```
might help



## Testing Trajectory Generation
In separate terminal windows
```
roscore
```

Generate the waypoints:
```
rosrun trajectory_generation test_trajectory_generation_node
```

Generate the trajectory:
```
rosrun trajectory_generation trajectory_generation_node 
```


Expected output:
```
[ INFO] [1617205726.876506837]: Starting Trajectory Generation Node
Received poseArray 
Received 17 desired Waypoints - generating trajectory
[ INFO] [1617205727.083034159]: Generated optimized trajectory from 18 waypoints
```

To see the pose array, we additionally need to run
```
rosrun tf static_transrm_publisher 0 0 0 0 0 0 1 map /world 10
```
(or maybe 
rosrun tf static_transform_publisher 0 0 0 0 0 0 1 map /world 10
)
``
and then
```
rviz
```

and then add the topic `/desired_trajectory_waypoints`



## Testing the combined cart + drone
In separate terminals:

Start the drone:
```
roslaunch rotors_gazebo mav_hovering_example.launch mav_name:=firefly world_name:=basic
```

Spawn the cart:
```
roslaunch vehicle_description spawn_cart_holonomic.launch 
```
in this terminal pressing 1 + enter makes the cart move forward

Launch the trajectory generation node:
```
rosrun trajectory_generation trajectory_generation_node 
```


## Testing the landing sequence
```
roslaunch interface landing_test.launch
```

## Testing the octomap generation
```
roslaunch interface landing_test.launch
```
```
rosservice call /world/get_octomap '{bounding_box_origin: {x: 5, y: 0, z: 1.75}, bounding_box_lengths: {x: 20.8, y: 20.8, z: 4}, leaf_size: 0.1, filename: output_filename.bt}'
```
reference: 
https://github.com/ethz-asl/rotors_simulator/wiki/Generate-an-octomap-from-your-world

rosservice call /world/get_octomap '{bounding_box_origin: {x: 30, y: 0, z: 5}, bounding_box_lengths: {x: 70, y: 50, z: 12}, leaf_size: 0.1, filename: /home/devansh/vna2v_project_ws/src/AE740_FINAL_PROJECT/path_planning/assets/road_2.bt}'

to visualize make sure octomap rviz plugins is installed, run:
```
rosservice call /world/get_octomap '{bounding_box_origin: {x: 5, y: 0, z: 1.75}, bounding_box_lengths: {x: 20.8, y: 20.8, z: 4}, leaf_size: 0.1, publish_octomap: true}'
```
and in rviz you should be able to see a topic /world/octomap 
dont forget to change the referernce frame from map to world


## Live generation of octomap:
```
roslaunch interface landing_test.launch
```

```
rosrun path_planning generate_octomap
```
this can be visualized in the rviz


## first goal position:
(5.0, 7.75, 0.75)

## Teleop
```
roslaunch interface landing_test.launch world_name:=outdoor
rosrun teleop_twist_keyboard teleop_twist_keyboard.py
rosrun vehicle_description move_van_holonomic
```


## Testing aruco markers:
```
roslaunch interface landing_test.launch
```

navigate somewhere nice and `rviz`

```
roslaunch aruco_ros single.launch
```
now launch something that requires an output from aruco detection (detection analysis stops otherwise)
and if the marker is detected, the pose will be published to `/aruco_single/pose`




## Testing landing with driving truck

```
roslaunch interface landing_test.launch
```
in a separate terminal
```
rosrun ackermann_drive_teleop keyop.py
```
this allows you to drive using `arrow keys`, stop with `spacebar` and reset speed and steering using `tab` 

Once the drone is launched you can tell it what to do:
```
rostopic pub /changeState
```
press `tab` enough times for autocomplete. 
Set the integer to:
```
1 - hover at current state
2 - plan RRT and visual land on van
5 - plan RRT and arrive at package drop location
```

To stop motors:
```
rostopic pub /firefly/arm std_msgs/Bool "data: false" 
```

To start motors:
```
rostopic pub /firefly/arm std_msgs/Bool "data: true"
```

the motors will start again when either the above true is sent or when any deisred pose is sent 



## Testing the full sequence
```
roslaunch interface landing_test.launch
```


## Todo:
1. Fix RRT/min snap collision avoidance
2. visual landing
3. add second drone
4. spawn drone on van rather than somewhere else
5. generate better octomaps
6. replan rrt if doesnt end at goal
