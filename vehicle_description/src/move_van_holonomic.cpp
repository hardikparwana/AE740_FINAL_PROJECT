#include <ros/ros.h>


#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Twist.h>
#include <gazebo_msgs/ModelState.h>
#include <gazebo_msgs/ModelStates.h>
#include <gazebo_msgs/SetModelState.h>
#include <iostream>

#include <sys/select.h>

#include <cmath>


using namespace std;

bool cartUpdated_ = false;
bool vel_updated_ = false;

double van_x = 2.0;
double van_y = 2.0;
double van_z = 0.05;
double van_yaw = 0;


void velCallback(geometry_msgs::Twist msg){
       
 	vel = msg;
}






void step_holonomic(double dt){

	van_x  	 += vel.linear.x  * dt;
	van_y  	 += vel.linear.y  * dt;
	van_z  	 += vel.linear.z  * dt;
	van_yaw  += vel.angular.z * dt;

}

void step_dubins(double dt){

	double vx = van_v * std::cos(van_yaw);
	double vy = van_v * std::sin(van_yaw);


	van_x +=  vx * dt;
	van_y +=  vy * dt;
	van_yaw += van_omega * dt;

	van_v =  vel.linear.x;
	van_omega = vel.angular.z;

	ROS_INFO_STREAM("x " << van_x  << " y " << van_y << " yaw " << van_yaw << " vx " << vx << " vy " << vy);
}

void step(double dt){
	// choose stepping mode
	step_dubins(dt);
}

int main(int argc, char** argv)
{
	ros::init(argc,argv, "move_van_holonomic");
	ros::NodeHandle n;

	ros::ServiceClient client = n.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");

	ros::Subscriber sub_vel = n.subscribe("/cmd_vel",1,velCallback);

	float dt = 0.001;
	
	ros::Rate r(1.0/dt);

	


	while(ros::ok())
	{

		// update internal state
		step(dt);

		gazebo_msgs::ModelState state;

		state.model_name =  (std::string) "ambulance";
		state.reference_frame = (std::string) "world";
	
		state.pose.position.x = van_x;
		state.pose.position.y = van_y;
		state.pose.position.z = van_z;

		// state.pose.quaternion.setRPY( 0, 0, van_yaw); to avoid importing tf/tf2
		state.pose.orientation.x = 0;
		state.pose.orientation.y = 0;
		state.pose.orientation.z = std::sin(0.5*van_yaw + 0.25*M_PI);
		state.pose.orientation.w = std::cos(0.5*van_yaw + 0.25*M_PI); 

		//state.pose.vel/ang_vel = 0 # since our update rate is high enough

 		gazebo_msgs::SetModelState setmodelstate;
		setmodelstate.request.model_state = state;
		client.call(setmodelstate);
		
		ros::spinOnce();
    	r.sleep();

	}


}


// int input;
		// cin >> input; 
		// if (input==1)
		// {
		// 	cmd_vel.linear.x = 0.3f;
		// 	cmd_vel.linear.y = 0.0f;
		// 	cmd_vel.angular.z = 0.0f;
		// }
		// else if (input==2)
		// {
		// 	cmd_vel.linear.x = 0.0f;
		// 	cmd_vel.linear.y = 0.3f;
		// 	cmd_vel.angular.z = 0.0f;//0.3f;
		// }	
		// else if (input==3)
		// {
		// 	cmd_vel.linear.x = 0.0f;
		// 	cmd_vel.linear.y = 0.0f;
		// 	cmd_vel.angular.z = 2.0f;//0.3f;
		// }	
		// else
		// {
		// 	cmd_vel.linear.x = 0.0f;
		// 	cmd_vel.linear.y = 0.0f;
		// 	cmd_vel.angular.z = 0.0f;
		// }	
 	// 	cout << "input: " << input << endl;
