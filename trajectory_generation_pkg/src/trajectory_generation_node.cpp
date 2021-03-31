

#include <ros/ros.h>
#include <eigen3/Eigen/Dense>

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseArray.h>

#include <mav_trajectory_generation/polynomial_optimization_linear.h>
#include <mav_trajectory_generation/trajectory.h>

#include <mav_trajectory_generation_ros/ros_visualization.h>

#include <ros/console.h>

#include <iostream>



class WaypointFollower{
    ros::Subscriber currentStateSub;
    ros::Subscriber desiredWaypointsSub;

    ros::Time trajectoryStartTime;
    mav_trajectory_generation::Trajectory trajectory;

    //Current State
    Eigen::Vector3d x;



    void onCurrentState(geometry_msgs::Pose const & cur_state){
        std::cout << "Received current state is: " << cur_state << std::endl;
    }

    void generateTrajectory(geometry_msgs::PoseArray const & poseArray){
        
        std::cout << "Received poseArray " << std::endl;
        
        if (poseArray.poses.size() < 1) {
            ROS_ERROR("Must have at least one pose to generate trajectory!");
            trajectory.clear();
            return;
        }

        int N = poseArray.poses.size();

        std::cout << "Received " << N << " desired Waypoints - generating trajectory" << std::endl;

        // clear trajectory
        trajectory.clear();

        const int D = 3; // dimension of each vertex

        mav_trajectory_generation::Vertex start_position(D), end_position(D);

        mav_trajectory_generation::Vertex::Vector vertices;

        // Start from the current position and zero orientation
        using namespace mav_trajectory_generation::derivative_order;
        start_position.makeStartOrEnd(x, SNAP);
        vertices.push_back(start_position); 

        for (auto i = 0; i < N; i++){
            // Collect data from the poseArray

            geometry_msgs::Point nextPos = poseArray.poses[i].position;

            Eigen::Vector3d nextPos_eigen(nextPos.x, nextPos.y, nextPos.z);
            mav_trajectory_generation::Vertex nextVert(D); 

            // if terminal state:  
            if (i == N-1){
                nextVert.makeStartOrEnd(nextPos_eigen, SNAP);
            }
            else{
                nextVert.addConstraint(POSITION, nextPos_eigen);
            }

            vertices.push_back(nextVert);

        }


        // allocate segment times
        std::vector<double> segment_times;
        const double v_max = 10;
        const double a_max = 100;

        // segment_times = estimateSegmentTimes(vertices, v_max, a_max);

        for (int i = 0; i < vertices.size()-1; i++){
            segment_times.push_back(1.0*(i+1));
        }

        // Solve for the trajectory
        mav_trajectory_generation::PolynomialOptimization<10> opt(D);
        opt.setupFromVertices(vertices, segment_times, SNAP);
        opt.solveLinear();

        // get the optimized trajectory
        mav_trajectory_generation::Segment::Vector segments;
        opt.getTrajectory(&trajectory);
        trajectoryStartTime = ros::Time::now();

        ROS_INFO("Generated optimized trajectory from %d waypoints", vertices.size());

        visualization_msgs::MarkerArray markers;
        double distance = 0.2; // Distance by which to seperate additional markers. Set 0.0 to disable.
        std::string frame_id = "world";

        // From Trajectory class:
        mav_trajectory_generation::drawMavTrajectory(trajectory, distance, frame_id, &markers);


    }

    public:
        explicit WaypointFollower(ros::NodeHandle& nh){
        
            currentStateSub = nh.subscribe(
                "/current_state", 1, &WaypointFollower::onCurrentState, this);

            desiredWaypointsSub = nh.subscribe(
                "/desired_trajectory_waypoints", 10, &WaypointFollower::generateTrajectory, this);
            

        }


};

int main(int argc, char** argv) {
  ros::init(argc, argv, "trajectory_generation_node");
  ros::NodeHandle nh;

  WaypointFollower waypointFollower(nh);

  ROS_INFO("Starting Trajectory Generation Node");

  ros::spin();
  return 0;
}
