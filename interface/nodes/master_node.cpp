#include <master_node.h>
#include <eigen3/Eigen/Dense>

// #include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Int8.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PointStamped.h>

#include <utils.h>


class StateMachine{

	ros::Subscriber currentStateSub;
	ros::Subscriber cartPoseSub;
	ros::Subscriber cartPoseVisualSub;
    ros::Subscriber stateChangeSub;
    ros::Subscriber bizTargetPointSub;


    ros::Publisher desPosePub;
    ros::Publisher goalPub;
    ros::Publisher commandCompletePub;



	//Current State
    Eigen::Vector3d current_state_;
    // double current_yaw;
    Eigen::Vector3d cart_state_;

    Eigen::Vector3d landing_spot_visual_;
    ros::Time landing_spot_visualRecTime;


    Eigen::Vector3d nextTargetPoint;
    Eigen::Vector3d realTargetPoint;


    int8_t state_ = 0;
    

    // bool stateChanged = false;
    bool droneUpdated_ = false;
    bool cartUpdated_ = false;

    void currentStateCallback(geometry_msgs::Pose const & cur_state){
        current_state_ << cur_state.position.x , cur_state.position.y , cur_state.position.z;
        droneUpdated_ = true;
        executeStateMachine();
    }
             
    void cartPoseCallback(gazebo_msgs::ModelStates modelState){
        int cartIndex = resolveGazeboIndex(modelState,std::string("ambulance"));
        geometry_msgs::Pose cartPose = modelState.pose[cartIndex];
        cart_state_ << cartPose.position.x , cartPose.position.y , cartPose.position.z;
        cartUpdated_ = true;
    }

    void cartPoseVisualCallback(geometry_msgs::PoseStamped const & targetPose){
        landing_spot_visual_ << targetPose.pose.position.x , targetPose.pose.position.y , targetPose.pose.position.z;
        landing_spot_visualRecTime = targetPose.header.stamp;
    }

    void receiveState(std_msgs::Int8 const & newState){
        changeStateTo(newState.data);
    }

    void changeStateTo(int8_t newState)
    {
        if (newState == state_){
            return;
        }
        else{
            std::cout << "Changing State from " << static_cast<int16_t>(state_) << " to " << static_cast<int16_t>(newState) << std::endl;
            state_ = newState;
            return;
        }
    }

    void executeStateMachine()
    {
        
    	int8_t newState = state_;

		switch(state_)
		{
			case exploration_status_t::STATE_INITIALIZING:
    			newState = executeInitializing();
                changeStateTo(newState);
                return;

            case exploration_status_t::STATE_HOVER:
                return;
            
            case exploration_status_t::STATE_PLAN_RRT_TO_VAN:
    			newState = planRRT_to_van();
                changeStateTo(newState);
                return;

             
    		case exploration_status_t::STATE_FOLLOW_RRT_TO_VAN:
    			newState = followRRT_to_van();
                changeStateTo(newState);
                return;

            // if near (within a cone of) the goal, then shift to pure control
    		case exploration_status_t::STATE_PROXIMITY_CONTROL:
    			newState = executeLandingControlVisual();
                changeStateTo(newState);
                return;
            
            case exploration_status_t::STATE_PLAN_RRT:
                newState = executeRRT_to_not_van();
                changeStateTo(newState);
                return;

            case exploration_status_t::STATE_FOLLOW_RRT:
                newState = execute_follow_rrt();
                changeStateTo(newState);
                return;
		}

        return;

    }

    int8_t executeInitializing()
    {
        // ROS_INFO(" *** EXECUTING INIT *** ");
        //wait for all messages to start coming
        if (!(droneUpdated_ && cartUpdated_)){
            return exploration_status_t::STATE_INITIALIZING;
        }

        // check that it is doing the initial hover correctly
        if (!checkInitialHovering()){
            return exploration_status_t::STATE_INITIALIZING;
        }

        // once initialized and hovering, we can choose the next landing mode, 
        // and return that as the new state
        // completed the initialization
        publishCommandComplete();
        return exploration_status_t::STATE_HOVER;
    }

    // @brief: plan a path to get to the proximity cone above the van
    // for now, just publish a desired waypoint above the van
    int8_t planRRT_to_van()
    {
        // ROS_INFO(" *** EXECUTING RRT *** ");

        nextTargetPoint << cart_state_[0], cart_state_[1], cart_state_[2] + 5.0;

        geometry_msgs::PointStamped goalPoint;

        goalPoint.point.x = nextTargetPoint[0];
        goalPoint.point.y = nextTargetPoint[1];
        goalPoint.point.z = nextTargetPoint[2];

        goalPoint.header.frame_id = "world";

        goalPub.publish(goalPoint);

        return exploration_status_t::STATE_FOLLOW_RRT_TO_VAN;

    }

    int8_t followRRT_to_van(){

        
        if (distancePoints(nextTargetPoint, current_state_) < 0.5){
            return choose_next_landing_mode();
        }

        if (checkMarkerIsRecent()){ 
            return exploration_status_t::STATE_PROXIMITY_CONTROL;
        }
        
        return exploration_status_t::STATE_FOLLOW_RRT_TO_VAN;

    }

    //    @bief: execute final landing onto van's roof
    //    @note: uses the trajectory generation pkg
    int8_t executeLandingControl()
    {
            // publish the landing pose to /desiredWaypoint
            // such that the quad will try to land on the van's roof

            geometry_msgs::Pose desPose;
            desPose.position.x = cart_state_[0];
            desPose.position.y = cart_state_[1];
            desPose.position.z = cart_state_[2] + 2.9;

            desPosePub.publish(desPose);

            return choose_next_landing_mode();

    }

    bool checkMarkerIsRecent(){

        return (ros::Time::now() - landing_spot_visualRecTime).toSec() < 2.0;

    }

    //    @bief: execute final landing onto van's roof
    //    @note: uses the trajectory generation pkg, and the aruco visual markers
    int8_t executeLandingControlVisual()
    {
            // publish the landing pose to /desiredWaypoint
            // such that the quad will try to land on the van's roof
            geometry_msgs::Pose desPose;
            
            // Convergence Rate
            float alpha = 0.5;

            bool markerIsRecent = checkMarkerIsRecent();
            
            if (markerIsRecent){
                desPose.position.x = landing_spot_visual_[0];
                desPose.position.y = landing_spot_visual_[1];
                desPose.position.z = (alpha) * landing_spot_visual_[2] + (1.0-alpha) * current_state_[2];
            }
            else{
                // fall back to non-visual landing
                return exploration_status_t::STATE_PLAN_RRT_TO_VAN;
            }
            desPosePub.publish(desPose);

            return choose_next_landing_mode();

    }



    int8_t choose_next_landing_mode()
    {
        // assumes initialization happened successfully 

        if (checkLanded(current_state_, landing_spot_visual_)){
            publishCommandComplete();
            return exploration_status_t::STATE_HOVER;
        }

        if (checkMarkerIsRecent()){ 
            return exploration_status_t::STATE_PROXIMITY_CONTROL;
        }

        // // tells if it is within a cone of goal location
        // bool proximity = checkDistanceToGoal(current_state_, cart_state_);
        // if (proximity){
        //     return exploration_status_t::STATE_PROXIMITY_CONTROL;
        // }

        return exploration_status_t::STATE_PLAN_RRT_TO_VAN;

    }

    bool checkInitialHovering(){

        // expected location for hovering
        Eigen::Vector3d expected_state(0, 0, 4.0);

        // compute distance
        float dist = (expected_state - current_state_).norm();

        if (dist < 0.25){
            return true;
        }

        return false;

    }

    int8_t executeRRT_to_not_van(){

        geometry_msgs::PointStamped goalPoint;

        goalPoint.point.x = nextTargetPoint[0];
        goalPoint.point.y = nextTargetPoint[1];
        goalPoint.point.z = nextTargetPoint[2];

        goalPoint.header.frame_id = "world";

        goalPub.publish(goalPoint);

        return exploration_status_t::STATE_FOLLOW_RRT;

    }


    int8_t execute_follow_rrt(){

        if (distancePoints(nextTargetPoint, current_state_) < 0.5){
            publishCommandComplete();
            return exploration_status_t::STATE_HOVER;
        }
        
        return exploration_status_t::STATE_FOLLOW_RRT;

    }

    void publishCommandComplete(){
        // publishes a message saying that the biz command was completed
        std_msgs::Bool msg;
        msg.data = true;
        commandCompletePub.publish(msg);
    }

    void newBizTargetCallback(geometry_msgs::Point const & msg){

        nextTargetPoint << msg.x, msg.y, msg.z;

        changeStateTo(exploration_status_t::STATE_PLAN_RRT);

    }


	public:
        explicit StateMachine(ros::NodeHandle& nh){

            currentStateSub = nh.subscribe(
                "/firefly/odometry_sensor1/pose", 1, &StateMachine::currentStateCallback, this);

            cartPoseSub = nh.subscribe(
                "/gazebo/model_states", 2, &StateMachine::cartPoseCallback, this);

            cartPoseVisualSub = nh.subscribe(
                "/aruco_single/pose", 2, &StateMachine::cartPoseVisualCallback, this);
            

            stateChangeSub = nh.subscribe("/changeState", 1, &StateMachine::receiveState, this);

            bizTargetPointSub = nh.subscribe("/bizTargetPoint", 1, &StateMachine::newBizTargetCallback, this);

            desPosePub = nh.advertise<geometry_msgs::Pose>("/desired_waypoint",1);
            
            goalPub = nh.advertise<geometry_msgs::PointStamped>("/goal_position", 1);

            commandCompletePub = nh.advertise<std_msgs::Bool>("/bizCommandComplete", 1);

            

        }
};



int main(int argc, char** argv)
{
	ros::init(argc,argv, "master_node");
	ros::NodeHandle nh;

	StateMachine operation_mode(nh);

	ROS_INFO("Starting Master Node");

	ros::spin();
  	return 0;
}