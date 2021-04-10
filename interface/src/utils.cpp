
#include <iostream>
#include <utils.h>

#define _USE_MATH_DEFINES
#include <cmath>

int resolveGazeboIndex( gazebo_msgs::ModelStates modelState, std::string name  )
{
	int index;
    for (int i=0; i<modelState.name.size()+1; i++){
        if (i == modelState.name.size()){ // no cart found
        	ROS_ERROR("Gazebo Name Resolution failed: No link named %s", name);
            return -1;
        }
        if (std::strcmp(modelState.name[i].c_str(), name.c_str()) == 0){
            index = i;
            break;
        }   
    }
    return index;
}

bool checkDistanceToGoal(Eigen::Vector3d robot, Eigen::Vector3d van)
{
	// Cone angle = 60 degrees
	float cone_angle = 60*M_PI/180.0f;

	// max distance
	float dist_thresold = 5.0f;
	float dist = distancePoints(robot,van);

	if (dist>dist_thresold)
		return false;

	//compute angle
	Eigen::Vector3d e3(0,0,1);
	Eigen::Vector3d line = robot - van;

	float angle = line.dot(e3);

	if (angle<cone_angle)
		return false;

	return true;

}

float distancePoints(Eigen::Vector3d p1, Eigen::Vector3d p2)
{
	return (p1-p2).norm();
}