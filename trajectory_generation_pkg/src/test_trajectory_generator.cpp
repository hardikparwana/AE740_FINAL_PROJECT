

#include <ros/ros.h>
#include <geometry_msgs/PoseArray.h>
#include <tf/transform_broadcaster.h>
#include <cmath>

// This code generates a poseArray and publishes it once
// it is published as soon as a node subscribes to /desired_trajectory_wapoints topic


int main(int argc, char** argv){
  ros::init(argc, argv, "test_waypoint_generator");
  ros::NodeHandle nh;

  tf::TransformBroadcaster br = tf::TransformBroadcaster();

  ros::Publisher posePublisher = nh.advertise<geometry_msgs::PoseArray>(
    "/desired_trajectory_waypoints", 1);
  
  geometry_msgs::PoseArray poseArray;

  geometry_msgs::Pose start;
  geometry_msgs::Pose mid;
  geometry_msgs::Pose end;

  start.position.x = 0.0;
  mid.position.x = -1;
  end.position.x = -2;

  poseArray.poses.push_back(start);
  poseArray.poses.push_back(mid);
  poseArray.poses.push_back(end);

  for (int i=0; i<14; i++){
    geometry_msgs::Pose pose;
    pose.position.x = i;
    pose.position.z = std::sin(M_PI*((1.0*i)/13.0));
    poseArray.poses.push_back(pose);
  }

  ROS_INFO("Waiting for subscriber to waypoints...");

  while (posePublisher.getNumSubscribers() == 0){
    ros::spinOnce();
  }

  poseArray.header.frame_id = "/world";

  ROS_INFO("Publishing Desired Waypoints");
  posePublisher.publish(poseArray);
  posePublisher.publish(poseArray);
  posePublisher.publish(poseArray);

  return 0;
  

}

