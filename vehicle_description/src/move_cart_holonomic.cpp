#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>

#include <eigen3/Eigen/Dense>

#include <geometry_msgs/Pose.h>
#include <gazebo_msgs/ModelStates.h>

#include <ackermann_msgs/AckermannDriveStamped.h>
#include <cmath>

using namespace std;

double yaw = 0;  // theta in the website
double delta = 0.0;
double speed = 0.0; // collected from ackerman msg
double steer = 0.0; // collected from ackerman msg

double vx = 0.0;
double vy = 0.0;
double yaw_dot = 0.0;

const double van_L = 4.5; 

bool cartUpdated_ = false;

Eigen::Vector3d cart_state_;

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

void cartPoseCallback(gazebo_msgs::ModelStates modelState){
        int cartIndex = resolveGazeboIndex(modelState,std::string("ambulance"));
        geometry_msgs::Pose cartPose = modelState.pose[cartIndex];
        cart_state_ << cartPose.position.x , cartPose.position.y , cartPose.position.z;
        cartUpdated_ = true;

	    yaw = std::atan2( cartPose.orientation.z, cartPose.orientation.w )*2;
    }

void steerCallback(ackermann_msgs::AckermannDriveStamped msg){
       
 	speed = msg.drive.speed;
    steer = msg.drive.steering_angle;

}

void step(){

    vx = speed * std::cos(yaw);
    vy = speed * std::sin(yaw);
    yaw_dot= std::abs(speed) * std::tan(steer) / van_L;

} 

int main(int argc, char** argv)
{
	ros::init(argc,argv, "move_cart_holonomic");
	ros::NodeHandle n;

	ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel",50);
	ros::Subscriber cmdSub = n.subscribe("/ackermann_cmd", 1, steerCallback);

	ros::Subscriber cartPoseSub = n.subscribe("/gazebo/model_states", 2, cartPoseCallback);

	ros::Rate r(100);

	geometry_msgs::Twist cmd_vel;
	
	cmd_vel.linear.y = 0.0f;
 	cmd_vel.linear.z = 0.0f;
 	cmd_vel.linear.x = 0.0f;
 	cmd_vel.angular.x = 0.0f;
    cmd_vel.angular.y = 0.0f;
    cmd_vel.angular.z = 0.0f;

	while(ros::ok())
	{
		
		step();

		cmd_vel.linear.x = vx;
		cmd_vel.linear.y = vy;
		cmd_vel.angular.z = yaw_dot;

 		pub.publish(cmd_vel);
		
		ros::spinOnce();
    	r.sleep();

	}


}


// int input;
// 		cin >> input; 
// 		if (input==1)
// 		{
// 			cmd_vel.linear.x = 1.0f;
// 			cmd_vel.linear.y = 0.0f;
// 			cmd_vel.angular.z = 0.0f;
// 		}
// 		else if (input==2)
// 		{
// 			cmd_vel.linear.x = 0.0f;
// 			cmd_vel.linear.y = 1.0f;
// 			cmd_vel.angular.z = 0.0f;//0.3f;
// 		}	
// 		else if (input==3)
// 		{
// 			cmd_vel.linear.x = 0.0f;
// 			cmd_vel.linear.y = 0.0f;
// 			cmd_vel.angular.z = 1.0f;//0.3f;
// 		}	
// 		else
// 		{
// 			cmd_vel.linear.x = 0.0f;
// 			cmd_vel.linear.y = 0.0f;
// 			cmd_vel.angular.z = 0.0f;
// 		}	
//  		cout << "input: " << input << endl;