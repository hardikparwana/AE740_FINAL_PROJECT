# AE740_FINAL_PROJECT
Final Project


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