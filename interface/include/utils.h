#include <ros/ros.h>
#include <ros/package.h>
#include <ros/console.h>

#include <geometry_msgs/Pose.h>
#include <gazebo_msgs/ModelStates.h>

#include <eigen3/Eigen/Dense>

#define _USE_MATH_DEFINES
#include <cmath>


int resolveGazeboIndex( gazebo_msgs::ModelStates modelState, std::string name  );

bool checkDistanceToGoal(Eigen::Vector3d robot, Eigen::Vector3d van);

float distancePoints(Eigen::Vector3d p1, Eigen::Vector3d p2);