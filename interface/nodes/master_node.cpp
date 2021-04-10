#include <master_node.h>
#include <eigen3/Eigen/Dense>

#include <utils.h>


class StateMachine{

	ros::Subscriber currentStateSub;
	ros::Subscriber cartPoseSub;

	//Current State
    Eigen::Vector3d current_state;
    double current_yaw;
    Eigen::Vector3d cart_state;

    int8_t state_;
    bool stateChanged = false;

    bool cartUpdated = false;

    void CurrentStateCallback(geometry_msgs::Pose const & cur_state){
        current_state << cur_state.position.x , cur_state.position.y , cur_state.position.z;
        cartUpdated = true;
    }
             
    void cartPoseCallback(gazebo_msgs::ModelStates modelState){
        int cartIndex = resolveGazeboIndex(modelState,std::string("cart"));
        geometry_msgs::Pose cartPose = modelState.pose[cartIndex];
        cart_state << cartPose.position.x , cartPose.position.y , cartPose.position.z;
        executeStateMachine();
    }

    void executeStateMachine()
    {
    	int8_t nextState = state_;
    	
		switch(state_)
		{
			case exploration_status_t::STATE_INITIALIZING:
    			nextState = executeInitializing();

            // if far away from goal, then do RRT
    		case exploration_status_t::STATE_REMOTE:
    			nextState = executeRRT();

            // if near (within a cone of) the goal, then shift to pure control
    		case exploration_status_t::STATE_PROXIMITY_CONTROL:
    			nextState = executeLandingControl();
		}

		stateChanged = nextState != state_;
    	state_ = nextState;
    }

    int8_t executeInitializing()
    {
        //wait for all messages to start coming
        if (!cartUpdated)
         return exploration_status_t::STATE_INITIALIZING;


        // tells if it is within a cone of goal location
        bool proximity = checkDistanceToGoal(current_state, cart_state);
        if (proximity)
            return exploration_status_t::STATE_PROXIMITY_CONTROL;
        return exploration_status_t::STATE_REMOTE;
    }

    int8_t executeRRT()
    {

        bool proximity = checkDistanceToGoal(current_state, cart_state);
        if (proximity)
            return exploration_status_t::STATE_PROXIMITY_CONTROL;
        return exploration_status_t::STATE_REMOTE;

    }

    int8_t executeLandingControl()
    {
            int y;
            y = 7;
    }

	public:
        explicit StateMachine(ros::NodeHandle& nh){

            currentStateSub = nh.subscribe(
                "/firefly/odometry_sensor1/pose", 1, &StateMachine::CurrentStateCallback, this);

            cartPoseSub = nh.subscribe(
                "/gazebo/model_states", 10, &StateMachine::cartPoseCallback, this);

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