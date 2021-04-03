# Vehcile Description

## Models Available
1. Holonomic with 4 wheels 
2. Unicycle with 4 wheels
3. Unicycle with two rear wheels and 1 castor wheel
**Note** Number of wheels is only for visualization for sake of this project. Kinematics has been taken care of separately for convenience.

## Controlling the Vehicles
Vehicles are controlled by publishing to a geometry_msgs/Twist message named **cmd_vel**. For holonomic systems linear X,Y and angular Z velocities(in world frame) can be set independently. For unicycle kinematics, only linear X and angular Z (in body frame) are set indepdently. All other fields in the message are 0.

## Testing models

1. Holonomic with 4 wheels

```
roslaunch vehicle_description spawn_cart_holonomic.launch 
```

If you have issues with VMware, when launching gazebo:
```
export SVGA_VGPU10=0
```
might help

* Move along X: write **1** in terminal and **Enter**
* Move along Y: write **2** in terminal and **Enter**
* Rotate along Z: write **3** in terminal and **Enter**
* Stop: write any other number in terminal and **Enter**

2. Unicycle with 4 wheels/ 2 wheels + Castor

```
roslaunch vehicle_description spawn_cart_unicycle.launch
```
OR
```
roslaunch vehicle_description spawn_castor.launch
```

* Move along Body X: write **1** in terminal and **Enter**
* Rotate along Z: write **2** in terminal and **Enter**
* Stop Motion: write any other number in terminal and **Enter**

