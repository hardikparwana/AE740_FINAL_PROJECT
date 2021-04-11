#include <ros/ros.h>
#include <ros/package.h>
#include <rotors_comm/Octomap.h>
#include <nav_msgs/Odometry.h>

// this node calls for the octomap to be regenerated every so often

double quad_x = 0;
double quad_y = 0;
double quad_z = 0;

void receivedQuadPos(const nav_msgs::Odometry  & msg){
    
    quad_x = msg.pose.pose.position.x;
    quad_y = msg.pose.pose.position.y;
    quad_z = msg.pose.pose.position.z;

}


int main(int argc, char ** argv){

    ros::init(argc, argv, "generate_octomap");
	ros::NodeHandle nh;
    
    ros::Subscriber vanPosSubscriber = nh.subscribe("/firefly/odometry_sensor1/odometry", 1, receivedQuadPos);

    ros::Rate rate(0.2);


    ros::ServiceClient client;
    client = nh.serviceClient<rotors_comm::Octomap>("world/get_octomap");

    while (ros::ok()){

        rotors_comm::Octomap msg;

        msg.request.bounding_box_origin.x = quad_x;
        msg.request.bounding_box_origin.y = quad_y;
        msg.request.bounding_box_origin.z = quad_z;

        msg.request.bounding_box_lengths.x = 20;
        msg.request.bounding_box_lengths.y = 20;
        msg.request.bounding_box_lengths.z = 10;

        msg.request.leaf_size = 0.1; // resolution of the map
        msg.request.publish_octomap = true;

        client.call(msg);

        ros::spinOnce();

        rate.sleep();
    }

    return 0;


}