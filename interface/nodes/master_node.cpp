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
	ros::Subscriber housePoseVisualSub;
    ros::Subscriber stateChangeSub;
    ros::Subscriber bizTargetPointSub;


    ros::Publisher desPosePub;
    ros::Publisher goalPub;
    ros::Publisher commandCompletePub;
    ros::Publisher motorArmPub;


	//Current State
    Eigen::Vector3d current_state_;
    // double current_yaw;
    Eigen::Vector3d cart_state_;
    Eigen::Vector3d cart_state_vel_;

    Eigen::Vector3d landing_spot_visual_;
    ros::Time landing_spot_visualRecTime;

    Eigen::Vector3d houselanding_spot_visual_;
    ros::Time houselanding_spot_visualRecTime;


    Eigen::Vector3d nextTargetPoint;
    Eigen::Vector3d realTargetPoint;


    int8_t state_ = 0;

    bool completedRRTtoVan = false;
    

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
        cart_state_vel_ << modelState.twist[cartIndex].linear.x , modelState.twist[cartIndex].linear.y, modelState.twist[cartIndex].linear.z;

        cartUpdated_ = true;
    }

    void cartPoseVisualCallback(geometry_msgs::PoseStamped const & targetPose){
        landing_spot_visual_ << targetPose.pose.position.x , targetPose.pose.position.y , targetPose.pose.position.z;
        landing_spot_visualRecTime = targetPose.header.stamp;
    }

    void housePoseVisualCallback(geometry_msgs::PoseStamped const & targetPose){
        houselanding_spot_visual_ << targetPose.pose.position.x , targetPose.pose.position.y , targetPose.pose.position.z;
        houselanding_spot_visualRecTime = targetPose.header.stamp;
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
                newState = planRRT_to_not_van();
                changeStateTo(newState);
                return;

            case exploration_status_t::STATE_FOLLOW_RRT:
                newState = followRRT_to_not_van();
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

        double T = (nextTargetPoint - current_state_).norm()/3.0;

        nextTargetPoint = nextTargetPoint + cart_state_vel_ * T;

        geometry_msgs::PointStamped goalPoint;
        goalPoint.point.x = nextTargetPoint[0];
        goalPoint.point.y = nextTargetPoint[1];
        goalPoint.point.z = nextTargetPoint[2];
        goalPoint.header.frame_id = "world";

        goalPub.publish(goalPoint);

        completedRRTtoVan = false;

        return exploration_status_t::STATE_FOLLOW_RRT_TO_VAN;

    }

    int8_t followRRT_to_van(){

        // if seen marker
        if (checkMarkerIsRecent()){ 
            return exploration_status_t::STATE_PROXIMITY_CONTROL;
        }
        

        if (!completedRRTtoVan){
            // if close to previously stated goal
            if (distancePoints(nextTargetPoint, current_state_) < 3.0){
                completedRRTtoVan = true;   
            }
            return exploration_status_t::STATE_FOLLOW_RRT_TO_VAN;
        }

        // if reached rrt, but no van sticker seen

        // dirty hack to get interpolated target point
        nextTargetPoint << cart_state_[0], cart_state_[1], cart_state_[2] + 5.0;
        
        double T = (nextTargetPoint - current_state_).norm()/3.0;
        nextTargetPoint = nextTargetPoint + cart_state_vel_ * T;

        Eigen::Vector3d dirToTarget = nextTargetPoint - current_state_;

        if (dirToTarget.norm() > 12.0){
            nextTargetPoint = current_state_ + 6.0 * dirToTarget/dirToTarget.norm();
        }
        else{
            nextTargetPoint = 0.5*(nextTargetPoint + current_state_);
        }

        geometry_msgs::Pose desPose;
        desPose.position.x = nextTargetPoint[0];
        desPose.position.y = nextTargetPoint[1];
        desPose.position.z = nextTargetPoint[2];

        desPosePub.publish(desPose);
        
        return exploration_status_t::STATE_FOLLOW_RRT_TO_VAN;

    }

    int8_t executeLandingControlVisual()
    {

            // publish the landing pose to /desiredWaypoint
            // such that the quad will try to land on the van's roof
            geometry_msgs::Pose desPose;
            
            // Convergence Rate
            // float alpha = 0.5;

            bool markerIsRecent = checkMarkerIsRecent();
            
            if (markerIsRecent){

                if (checkLanded(current_state_, getMarkerLocation())){
                    // disarmMotors();
                    publishCommandComplete();
                    return exploration_status_t::STATE_HOVER;
                }

                double ex = landing_spot_visual_[0] - current_state_[0];
                double ey = landing_spot_visual_[1] - current_state_[1];
                double ez = landing_spot_visual_[2] - current_state_[2];

                Eigen::Vector3d e;
                e << 0.7*ex , 0.7*ey, 0.7*ez;

                nextTargetPoint = current_state_ + e;

                double T = (nextTargetPoint - current_state_).norm()/1.0;

                nextTargetPoint = nextTargetPoint + cart_state_vel_ * T;

                desPose.position.x = nextTargetPoint[0];
                desPose.position.y = nextTargetPoint[1];
                desPose.position.z = nextTargetPoint[2];

                desPosePub.publish(desPose);
                return exploration_status_t::STATE_PROXIMITY_CONTROL;
                
            }
            ROS_INFO("In vis landing, Marker not recent ");
            return exploration_status_t::STATE_FOLLOW_RRT_TO_VAN;
    }

    int8_t planRRT_to_not_van(){

        geometry_msgs::PointStamped goalPoint;

        goalPoint.point.x = nextTargetPoint[0];
        goalPoint.point.y = nextTargetPoint[1];
        goalPoint.point.z = nextTargetPoint[2];

        goalPoint.header.frame_id = "world";

        goalPub.publish(goalPoint);

        return exploration_status_t::STATE_FOLLOW_RRT;

    }

    int8_t followRRT_to_not_van(){

        // if (distancePoints(nextTargetPoint, current_state_) < 1.0){
        //     disarmMotors();
        //     ros::Duration(3.0).sleep();
        //     publishCommandComplete();
        //     return exploration_status_t::STATE_HOVER;
        // }

        bool markerIsRecent = checkHouseMarkerIsRecent();
            
        if (markerIsRecent){

            if ((houselanding_spot_visual_ - current_state_).norm() < 0.5){
                // landed
                // disarmMotors();
                // ros::Duration(3.0).sleep();
                publishCommandComplete();
                return exploration_status_t::STATE_HOVER;
            }

            // marker seen visual landing
            double ex = houselanding_spot_visual_[0] - current_state_[0];
            double ey = houselanding_spot_visual_[1] - current_state_[1];
            double ez = houselanding_spot_visual_[2] - current_state_[2];

            geometry_msgs::Pose desPose;

            desPose.position.x = current_state_[0] + 0.8 * ex;
            desPose.position.y = current_state_[1] + 0.8 * ey;
            desPose.position.z = current_state_[2] + 0.4 * ez;
            desPosePub.publish(desPose);
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

    void disarmMotors(){
        std_msgs::Bool msg;
        msg.data = false;
        motorArmPub.publish(msg);
    }
    
    void armMotors(){
        std_msgs::Bool msg;
        msg.data = true;
        motorArmPub.publish(msg);
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

    bool checkMarkerIsRecent(){
        return (ros::Time::now() - landing_spot_visualRecTime).toSec() < 2.0;
    }

    bool checkHouseMarkerIsRecent(){
        return (ros::Time::now() - houselanding_spot_visualRecTime).toSec() < 2.0;
    }

    Eigen::Vector3d getMarkerLocation(){
        Eigen::Vector3d offset;
        offset << 0.0, -0.8, 3.0;
        ROS_INFO_STREAM("CART: " << cart_state_ << " True Marker " << cart_state_ + offset << " Drone: " << current_state_ << " Vis" << landing_spot_visual_);
        return cart_state_ + offset;
    }


	public:
        explicit StateMachine(ros::NodeHandle& nh){

            currentStateSub = nh.subscribe(
                "/firefly/odometry_sensor1/pose", 1, &StateMachine::currentStateCallback, this);

            cartPoseSub = nh.subscribe(
                "/gazebo/model_states", 2, &StateMachine::cartPoseCallback, this);

            cartPoseVisualSub = nh.subscribe(
                "/aruco_single/pose_26", 2, &StateMachine::cartPoseVisualCallback, this);

            housePoseVisualSub = nh.subscribe(
                "/aruco_single/pose_582", 2, &StateMachine::housePoseVisualCallback, this);
            

            stateChangeSub = nh.subscribe("/changeState", 1, &StateMachine::receiveState, this);

            bizTargetPointSub = nh.subscribe("/bizTargetPoint", 1, &StateMachine::newBizTargetCallback, this);

            desPosePub = nh.advertise<geometry_msgs::Pose>("/desired_waypoint",1);
            
            goalPub = nh.advertise<geometry_msgs::PointStamped>("/goal_position", 1);

            commandCompletePub = nh.advertise<std_msgs::Bool>("/bizCommandComplete", 1);

            motorArmPub = nh.advertise<std_msgs::Bool>("/firefly/arm",1);
            

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