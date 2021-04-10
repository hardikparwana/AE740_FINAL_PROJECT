

#include <ros/ros.h>
#include <eigen3/Eigen/Dense>

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseArray.h>
#include <trajectory_msgs/MultiDOFJointTrajectory.h>

#include <gazebo_msgs/ModelStates.h>

#include <tf/transform_datatypes.h>

#include <mav_trajectory_generation/polynomial_optimization_linear.h>
#include <mav_trajectory_generation/trajectory.h>

#include <mav_trajectory_generation_ros/ros_visualization.h>
#include <mav_trajectory_generation/trajectory_sampling.h>

#include <mav_msgs/conversions.h>

#include <ros/console.h>

#include <iostream>
#include <algorithm>
#include <iterator>

class WaypointFollower{
    ros::Subscriber currentStateSub;
    ros::Subscriber desiredWaypointsSub;
    ros::Subscriber desiredWaypointSingleSub;
    ros::Subscriber cartPoseSub;

    ros::Time trajectoryStartTime;
    mav_trajectory_generation::Trajectory trajectory;

    ros::Publisher trajectoryPubMarkers;

    ros::Publisher trajectoryPub;
    ros::Publisher desPosePub;

    //Current State
    Eigen::Vector3d current_state;
    double current_yaw;

    ros::Time lastReplan;
    bool runTraj = true;



    void onCurrentState(geometry_msgs::Pose const & cur_state){
        
        current_state << cur_state.position.x , cur_state.position.y , cur_state.position.z;

        //std::cout << "Recieved Current State: " << cur_state << std::endl;
        //std::cout << "stored Current State:   " << current_state << std::endl;
        // current_yaw = tf::getYaw( tf::Quaternion( cur_state.orientation.x, cur_state.orientation.y, cur_state.orientation.z, cur_state.orientation.w ) ); 

    }

    void generateTrajectorySingle(geometry_msgs::Pose const & pose){

        geometry_msgs::PoseStamped stampedPose;

        stampedPose.pose = pose;

        desPosePub.publish(stampedPose);

    }

    void goToCart(gazebo_msgs::ModelStates modelState){

        // if (runTraj == false){
        //     return;
        // }
        if ((ros::Time::now().toSec() - lastReplan.toSec()) < 5){
            return;
        }

        lastReplan = ros::Time::now();
        // runTraj = false;
        
        int index = 0;
        std::string cartString = "cart";

        for (int i=0; i<modelState.name.size()+1; i++){
            if (i == modelState.name.size()){
                // no cart found
                return;
            }

            if (std::strcmp(modelState.name[i].c_str(), cartString.c_str()) == 0){
                index = i;
                break;
            }
            
        }

        geometry_msgs::Pose cartPose = modelState.pose[index];

        std::cout << "Received cart state is: " << cartPose << std::endl;

        // genarate poseArray 

        geometry_msgs::Pose aboveCart;
        aboveCart.position.x = cartPose.position.x;
        aboveCart.position.y = cartPose.position.y;
        aboveCart.position.z = 1.0;//std::max(current_state[2], 1.0);


        geometry_msgs::PoseArray poseArray;

        poseArray.poses.push_back(aboveCart);
        poseArray.poses.push_back(cartPose);

        generateTrajectory(poseArray);

    }

    void generateTrajectory(geometry_msgs::PoseArray const & poseArray){
        
        std::cout << "Received poseArray: " << poseArray << std::endl;
        
        if (poseArray.poses.size() < 1) {
            ROS_ERROR("Must have at least one pose to generate trajectory!");
            trajectory.clear();
            return;
        }

        int N = poseArray.poses.size();

        // std::cout << "Received " << N << " desired Waypoints - generating trajectory" << std::endl;

        // clear trajectory
        trajectory.clear();

        const int D = 3; // dimension of each vertex

        mav_trajectory_generation::Vertex start_position(D), end_position(D);

        mav_trajectory_generation::Vertex::Vector vertices;

        // Start from the current position and zero orientation
        using namespace mav_trajectory_generation::derivative_order;
        start_position.makeStartOrEnd(current_state, SNAP);
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
        const double v_max = 2.0;
        const double a_max = 5.0;

        segment_times = mav_trajectory_generation::estimateSegmentTimes(vertices, v_max, a_max);

        // for (int i = 0; i < vertices.size()-1; i++){
        //     segment_times.push_back(1.0*(i+1));
        // }

        // Solve for the trajectory
        mav_trajectory_generation::PolynomialOptimization<10> opt(D);
        opt.setupFromVertices(vertices, segment_times, SNAP);
        opt.solveLinear();

        // get the optimized trajectory
        mav_trajectory_generation::Segment::Vector segments;
        opt.getTrajectory(&trajectory);
        trajectoryStartTime = ros::Time::now();

        ROS_INFO("Generated optimized trajectory from %d waypoints", vertices.size());

        // send trajectory to rotorsSimulator

        // sample the trajectory:
        mav_msgs::EigenTrajectoryPoint::Vector trajectoryStates;
        double sampling_interval = 0.1;
        bool success = mav_trajectory_generation::sampleWholeTrajectory(trajectory, sampling_interval, &trajectoryStates);

        // convert to MultiDOFJointTrajectory Msg
        trajectory_msgs::MultiDOFJointTrajectory trajectoryMsg;
        mav_msgs::msgMultiDofJointTrajectoryFromEigen (trajectoryStates, &trajectoryMsg);

        // publish to /firelfy/command/trajectory
        trajectoryPub.publish(trajectoryMsg);

        // ** VIZ [start] **
        visualization_msgs::MarkerArray markers;
        double distance = 0.2; // Distance by which to seperate additional markers. Set 0.0 to disable.
        std::string frame_id = "/world";

        // From Trajectory class:
        mav_trajectory_generation::drawMavTrajectory(trajectory, distance, frame_id, &markers);

        trajectoryPubMarkers.publish(markers);

        // ** VIZ [end] **

    }

    public:
        explicit WaypointFollower(ros::NodeHandle& nh){

            lastReplan = ros::Time::now();
        
            currentStateSub = nh.subscribe(
                "/firefly/odometry_sensor1/pose", 1, &WaypointFollower::onCurrentState, this);

            desiredWaypointsSub = nh.subscribe(
                "/desired_trajectory_waypoints", 10, &WaypointFollower::generateTrajectory, this);

            desiredWaypointSingleSub = nh.subscribe(
                "/desired_waypoint", 10, &WaypointFollower::generateTrajectorySingle, this);

            // cartPoseSub = nh.subscribe(
            //     "/gazebo/model_states", 10, &WaypointFollower::goToCart, this);
            

            trajectoryPubMarkers = nh.advertise<visualization_msgs::MarkerArray>("/trajectory",20);

            trajectoryPub = nh.advertise<trajectory_msgs::MultiDOFJointTrajectory>("/firefly/command/trajectory",20);

            desPosePub = nh.advertise<geometry_msgs::PoseStamped>("/firefly/command/pose",20);

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
