#include <ros/ros.h>
#include <ros/package.h>
#include <iostream>

#include <geometry_msgs/Pose.h>
#include <gazebo_msgs/ModelStates.h>

namespace exploration_status_t
{
		static const int8_t   STATE_INITIALIZING = 0;
		static const int8_t   STATE_HOVER  = 1;
		static const int8_t   STATE_PLAN_RRT_TO_VAN = 2;
		static const int8_t   STATE_FOLLOW_RRT_TO_VAN = 3;                      // RRT to van
		static const int8_t   STATE_PROXIMITY_CONTROL = 4;           // landing on van
		static const int8_t   STATE_PLAN_RRT = 5;						 // RRT to non-van target
		static const int8_t   STATE_FOLLOW_RRT = 6;

}


