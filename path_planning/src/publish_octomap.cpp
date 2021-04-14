#include "ros/ros.h"
#include <octomap_msgs/Octomap.h>
#include <octomap_msgs/conversions.h>
#include <octomap_ros/conversions.h>
#include <octomap/octomap.h>

#include <iostream>


ros::Publisher mapPub;


void loadOctomap(){ 

    // const std::string filename = "/home/devansh/vna2v_project_ws/src/AE740_FINAL_PROJECT/path_planning/assets/output_filename_med.bt";

    // const std::string filename = "/home/devansh/vna2v_project_ws/src/AE740_FINAL_PROJECT/outdoor_world.bt";
    const std::string filename = "/home/devansh/vna2v_project_ws/src/AE740_FINAL_PROJECT/path_planning/output_filename_complete.bt";
    
    ROS_INFO_STREAM("Starting to load OctMap from " << filename);
    
    octomap::OcTree octree(0.1);
	bool suc = octree.readBinary(filename);

    if (!suc){
        ROS_WARN("READ BINARY NOT SUCCESFUL!");
    }

    // octomap::OcTree* octree = new octomap::OcTree(filename);
	// fcl::OcTree* tree = new fcl::OcTree(std::shadesired_trajectory_waypointsred_ptr<const octomap::OcTree>(&temp_tree));
	ROS_INFO("Created OcTree");
    octomap_msgs::Octomap bmap_msg;
    octomap_msgs::binaryMapToMsg(octree, bmap_msg);
    ROS_INFO("Converted to msg");

    bmap_msg.header.frame_id = "world";

    mapPub.publish(bmap_msg);

    ROS_INFO("Published Octomap!");

}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "octomap_publisher");
	ros::NodeHandle nh;

    ROS_INFO("Launching Octomap Publisher...");

	// ros::Subscriber octree_sub = n.subscribe<octomap_msgs::Octomap>("/world/octomap", 1, boost::bind(&octomapCallback, _1, &planner_object));

	mapPub = nh.advertise<octomap_msgs::Octomap>( "/world/octomap", 1 );

    ROS_INFO("Waiting for a subscriber...");
    while( mapPub.getNumSubscribers() < 1){
        ros::spinOnce();
    }

    ROS_INFO("Subscriber found! Publishing Octomap");

    loadOctomap();
	
	ros::spin();

	return 0;
}
