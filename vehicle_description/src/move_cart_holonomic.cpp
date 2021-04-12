#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	ros::init(argc,argv, "move_cart_holonomic");
	ros::NodeHandle n;

	ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_holonomic",50);

	ros::Rate r(50);

	geometry_msgs::Twist cmd_vel;
	
	cmd_vel.linear.y = 0.0f;
 	cmd_vel.linear.z = 0.0f;
 	cmd_vel.linear.x = 0.0f;
 	cmd_vel.angular.x = 0.0f;
    cmd_vel.angular.y = 0.0f;
    cmd_vel.angular.z = 0.0f;

	while(ros::ok())
	{

		int input;
		cin >> input; 
		if (input==1)
		{
			cmd_vel.linear.x = 0.3f;
			cmd_vel.linear.y = 0.0f;
			cmd_vel.angular.z = 0.0f;
		}
		else if (input==2)
		{
			cmd_vel.linear.x = 0.0f;
			cmd_vel.linear.y = 0.3f;
			cmd_vel.angular.z = 0.0f;//0.3f;
		}	
		else if (input==3)
		{
			cmd_vel.linear.x = 0.0f;
			cmd_vel.linear.y = 0.0f;
			cmd_vel.angular.z = 2.0f;//0.3f;
		}	
		else
		{
			cmd_vel.linear.x = 0.0f;
			cmd_vel.linear.y = 0.0f;
			cmd_vel.angular.z = 0.0f;
		}	
 		cout << "input: " << input << endl;

 		pub.publish(cmd_vel);
		
    	r.sleep();
	}


}