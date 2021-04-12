#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <gazebo_msgs/ModelState.h>
#include <gazebo_msgs/ModelStates.h>
#include <gazebo_msgs/SetModelState.h>
#include <iostream>

#include <sys/select.h>


#include <geometry_msgs/Pose.h>

using namespace std;

bool cartUpdated_ = false;
bool vel_updated_ = false;
geometry_msgs::Pose cartPose;

geometry_msgs::Twist vel;

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
        cartPose = modelState.pose[cartIndex];
        
        cartUpdated_ = true;

    }

 void velCallback(geometry_msgs::Twist msg){
       
 	vel = msg;
    }

int main(int argc, char** argv)
{
	ros::init(argc,argv, "move_van_holonomic");
	ros::NodeHandle n;

	ros::ServiceClient client = n.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");
	ros::Subscriber sub = n.subscribe("/gazebo/model_states",1,cartPoseCallback);

	ros::Subscriber sub_vel = n.subscribe("/cmd_vel",1,velCallback);

	ros::Rate r(50);

	gazebo_msgs::ModelState state;

	state.model_name =  (std::string) "ambulance";
	state.reference_frame = (std::string) "world";


	while(ros::ok())
	{

		if (!cartUpdated_){
			ros::spinOnce();
			continue;
		}


 		state.pose = cartPose;
 		state.twist = vel;

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
