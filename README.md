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
This should contain a copy of all the dependencies we need, so hopefully we dont need to run extra installation processes. 

From the workspace folder, we just need to build the folder:
```
~/catkin_ws$ catkin build -j8
```

and now hopefully all the dependencies can be built.




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


## Teleop
```
roslaunch interface landing_test.launch world_name:=outdoor
rosrun teleop_twist_keyboard teleop_twist_keyboard.py
rosrun vehicle_description move_van_holonomic
```