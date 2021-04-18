#include <ros/ros.h>

// #include <geometry_msgs/Pose.h>
#include <gazebo_msgs/ModelState.h>
#include <gazebo_msgs/SetModelState.h>
#include<ackermann_msgs/AckermannDriveStamped.h>

#include <cmath>

// equations from section 2.1 of https://dingyan89.medium.com/simple-understanding-of-kinematic-bicycle-model-81cac6420357

ros::ServiceClient client;


const double dt = 0.001;
const double van_z = 0.5;
const double van_L = 4.5; 
const double deltaMax = 0.25*M_PI;

double xr = 0.0;
double yr = 2.0;
double yaw = 0;  // theta in the website
double delta = 0.0;
double speed = 0.0; // collected from ackerman msg
double steer = 0.0; // collected from ackerman msg

double vx = 0.0;
double vy = 0.0;
double yaw_dot = 0.0;


void steerCallback(ackermann_msgs::AckermannDriveStamped msg){
       
 	speed = msg.drive.speed;
    steer = msg.drive.steering_angle;

}



void step(){

    vx = speed * std::cos(yaw);
    vy = speed * std::sin(yaw);
    yaw_dot= std::abs(speed) * std::tan(delta) / van_L;
    // double delta_dot = 3*steer;

    // update state
    xr += vx * dt;
    yr += vy * dt;
    yaw += yaw_dot * dt;
    // delta += delta_dot * dt;
    delta = steer;

    delta = std::max(-deltaMax, std::min(deltaMax, delta));

} 


void genMsg(){

    gazebo_msgs::ModelState state;

    state.model_name =  (std::string) "ambulance";
    state.reference_frame = (std::string) "world";

    // specify the center of the van for x and y
    state.pose.position.x = xr + 1.6 * std::cos(yaw);
    state.pose.position.y = yr + 1.6 * std::sin(yaw);
    
    // specify the bottom of the van
    state.pose.position.z = van_z;

    // state.pose.quaternion.setRPY( 0, 0, van_yaw); to avoid importing tf/tf2
    state.pose.orientation.x = 0;
    state.pose.orientation.y = 0;
    state.pose.orientation.z = std::sin(0.5*(yaw + 0.5*M_PI));
    state.pose.orientation.w = std::cos(0.5*(yaw + 0.5*M_PI)); 

    state.twist.linear.x = vx;
    state.twist.linear.y = vy;
    state.twist.angular.z = yaw_dot;

    gazebo_msgs::SetModelState setmodelstate;

    setmodelstate.request.model_state = state;
    
    client.call(setmodelstate);






}

// void setState(geometry_msgs::Pose msg){

//     yaw = msg.orientation.z;   
//  	xr = msg.position.x - van_L * std::cos(yaw);
//  	yr = msg.position.y - van_L * std::sin(yaw);
//     delta = 0;
//     genMsg();    

// }




int main(int argc, char** argv){

    ros::init(argc, argv, "drive_van");
    ros::NodeHandle nh;

    client = nh.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");
	ros::Subscriber cmdSub = nh.subscribe("/ackermann_cmd", 1, steerCallback);
	// nh.subscribe("/vanSetState", 1, setState);


	ros::Rate r(1.0/dt);

    while(ros::ok()){

        step();
        genMsg();

		ros::spinOnce();
    	r.sleep();
    }

}