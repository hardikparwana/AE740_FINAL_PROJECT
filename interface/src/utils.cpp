
#include <iostream>
#include <utils.h>

#define _USE_MATH_DEFINES
#include <cmath>

// returns the index at which a gazebo object named "name" is located in the gazebo pose message
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

// returns true if within a cone of goal, false otherwise
bool checkDistanceToGoal(Eigen::Vector3d robot, Eigen::Vector3d van)
{
	// Cone angle = 60 degrees
	float cos_cone_angle = 0.5; //std::cos(60*M_PI/180.0f);

	// max distance
	float dist_thresold = 10.0f;
	float dist = distancePoints(robot,van);

	if (dist>dist_thresold)
		return false;

	//compute angle
	Eigen::Vector3d e3(0,0,1);
	Eigen::Vector3d line = robot - van;

	// std::cout << "ROBOT: " << robot << " VAN: " << van << " Line: " << line << std::endl;

	float cos_angle = line.dot(e3)/(line.norm());

	// std::cout << "Cos Angle: " << cos_angle << " -- Cos 60: " <<  cos_cone_angle <<  " RETURNING: " <<  (cos_angle  > cos_cone_angle)<< std::endl; 

	if (cos_angle  > cos_cone_angle)
		return true;

	return false;

}

// returns distance between 2 points in R^3
float distancePoints(Eigen::Vector3d p1, Eigen::Vector3d p2)
{
	return (p1-p2).norm();
}

bool checkLanded(Eigen::Vector3d robot, Eigen::Vector3d van)
{

	// if (!checkDistanceToGoal(robot,van)){
	// 	return false;
	// }
	if ((robot-van).norm() < 0.5) {
		if (std::abs((robot-van)[2]) < 0.2){
			return true;
		}
	}
	return false;
}