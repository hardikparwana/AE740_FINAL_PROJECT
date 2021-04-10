#include <ros/ros.h>
#include <ros/package.h>
#include <iostream>

#include <geometry_msgs/Pose.h>
#include <gazebo_msgs/ModelStates.h>

namespace exploration_status_t
{
		static const int8_t   STATE_INITIALIZING = 0;
		static const int8_t   STATE_REMOTE = 1;                      // RRT
		static const int8_t   STATE_PROXIMITY_CONTROL = 2;           // Direct Control
}


