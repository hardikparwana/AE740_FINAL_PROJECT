#include "ros/ros.h"
#include <octomap_msgs/Octomap.h>
#include <octomap_msgs/conversions.h>
#include <octomap_ros/conversions.h>
#include <octomap/octomap.h>

#include <iostream>


ros::Publisher mapPub;


std::string filename;

void loadOctomap(){ 

    ROS_INFO_STREAM("Starting to load OctMap from " << filename);
    
    octomap::OcTree octree(0.1);
	bool suc = octree.readBinary(filename);

    if (!suc){
        ROS_ERROR("READ BINARY NOT SUCCESFUL! KILLING PUBLISH_OCTOMAP_NODE! ");
        ros::shutdown();
    }

	ROS_INFO("Created OcTree");
    octomap_msgs::Octomap bmap_msg;
    octomap_msgs::binaryMapToMsg(octree, bmap_msg);
    ROS_INFO("Converted to msg");

    bmap_msg.header.frame_id = "world";

    mapPub.publish(bmap_msg);

    ROS_INFO("Published Octomap!");

}

void getFileName(){
    std::string file;
    std::string fileDir;
    nh.getParam("octomapFileDir", fileDir);
    nh.getParam("octomapFile", file);
    filename = fileDir + file;
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "octomap_publisher");
	ros::NodeHandle nh;

    ROS_INFO("Launching Octomap Publisher...");

    getFileName();

    // checks for new subscribers once every second    
    ros::Rate rate(1);

    int numSub = 0;
	mapPub = nh.advertise<octomap_msgs::Octomap>( "/world/octomap", 1 );

    ROS_INFO("Waiting for a subscriber...");
    while(ros::ok() ){

        if (mapPub.getNumSubscribers() != numSub){

            numSub= mapPub.getNumSubscribers();
            
            ROS_INFO("Subscriber count changed! Publishing Octomap");
            
            loadOctomap();
        }

        ros::spinOnce();
        rate.sleep();
    }

	return 0;
}
