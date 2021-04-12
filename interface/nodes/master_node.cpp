#include <master_node.h>
#include <eigen3/Eigen/Dense>

#include <geometry_msgs/Pose.h>

#include <utils.h>


class StateMachine{

	ros::Subscriber currentStateSub;
	ros::Subscriber cartPoseSub;

    ros::Publisher desPosePub;

	//Current State
    Eigen::Vector3d current_state_;
    // double current_yaw;
    Eigen::Vector3d cart_state_;

    int8_t state_ = 0;
    

    // bool stateChanged = false;
    bool droneUpdated_ = false;
    bool cartUpdated_ = false;

    void currentStateCallback(geometry_msgs::Pose const & cur_state){
        current_state_ << cur_state.position.x , cur_state.position.y , cur_state.position.z;
        droneUpdated_ = true;
    }
             
    void cartPoseCallback(gazebo_msgs::ModelStates modelState){
        int cartIndex = resolveGazeboIndex(modelState,std::string("ambulance"));
        geometry_msgs::Pose cartPose = modelState.pose[cartIndex];
        cart_state_ << cartPose.position.x , cartPose.position.y , cartPose.position.z;
        cartUpdated_ = true;

        executeStateMachine();
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

            //if far away from goal, then do RRT
    		case exploration_status_t::STATE_REMOTE:
    			newState = executeRRT();
                changeStateTo(newState);
                return;

            // if near (within a cone of) the goal, then shift to pure control
    		case exploration_status_t::STATE_PROXIMITY_CONTROL:
    			newState = executeLandingControl();
                changeStateTo(newState);
                return;
		}

        return;

    }

    int8_t executeInitializing()
    {
        ROS_INFO(" *** EXECUTING INIT *** ");
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
        return choose_next_landing_mode();
    }

    // @brief: plan a path to get to the proximity cone above the van
    // for now, just publish a desired waypoint above the van
    int8_t executeRRT()
    {
        ROS_INFO(" *** EXECUTING RRT *** ");

        geometry_msgs::Pose desPose;
        desPose.position.x = cart_state_[0];
        desPose.position.y = cart_state_[1];
        desPose.position.z = cart_state_[2] + 4.0;

        desPosePub.publish(desPose);

        return choose_next_landing_mode();

    }

    //    @bief: execute final landing onto van's roof
    //    @note: uses the trajectory generation pkg
    int8_t executeLandingControl()
    {
            // publish the landing pose to /desiredWaypoint
            // such that the quad will try to land on the van's roof

            ROS_INFO(" *** EXECUTING LANDING CONTROL *** ");

            geometry_msgs::Pose desPose;
            desPose.position.x = cart_state_[0];
            desPose.position.y = cart_state_[1];
            desPose.position.z = cart_state_[2] + 2.9;

            desPosePub.publish(desPose);

            return choose_next_landing_mode();

    }

    int8_t choose_next_landing_mode()
    {
        // assumes initialization happened successfully 

        // tells if it is within a cone of goal location
        bool proximity = checkDistanceToGoal(current_state_, cart_state_);
        if (proximity){
            return exploration_status_t::STATE_PROXIMITY_CONTROL;
        }

        return exploration_status_t::STATE_REMOTE;

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


	public:
        explicit StateMachine(ros::NodeHandle& nh){

            currentStateSub = nh.subscribe(
                "/firefly/odometry_sensor1/pose", 1, &StateMachine::currentStateCallback, this);

            cartPoseSub = nh.subscribe(
                "/gazebo/model_states", 10, &StateMachine::cartPoseCallback, this);

            desPosePub = nh.advertise<geometry_msgs::Pose>("/desired_waypoint",20);

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