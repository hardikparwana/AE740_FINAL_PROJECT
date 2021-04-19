#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Int8.h>
#include <geometry_msgs/Point.h>


class BusinessNode{

    int numHouses = 0; 
    int nextHouse = 0;
    // houses is a vector of vectors
    std::vector<std::vector<double>> houses;


    ros::Subscriber bizCommandCompleteSub;
    ros::Publisher changeStatePub;
    ros::Publisher bizTargetPub;

    bool commandComplete = false;




    void loadHouses(ros::NodeHandle & nh){
        
        // load a single long vector of house xyz coordinates
        std::vector<double> housesTemp;
        nh.getParam("houses", housesTemp);

        // check size
        if (housesTemp.size() % 3 != 0){
            ROS_ERROR("Passed argument houses does not have 3N elements");
            ros::shutdown();
        }

        // save into vector of vectors for convenience
        numHouses = housesTemp.size()/3;

        for (int i=0; i<numHouses; i++){
            std::vector<double> house;
            house.push_back(housesTemp[3 * i] );
            house.push_back(housesTemp[3 * i + 1] );
            house.push_back(housesTemp[3 * i + 2] );
            houses.push_back(house);
        }

        // print houses
        ROS_INFO("LOADED HOUSES:");
        for (int i=0; i<numHouses; i++){
            ROS_INFO_STREAM("   House " << i << ": " << houses[i][0] << ", " <<  houses[i][1] << ", " << houses[i][2]);
        }

    }

    void publishNextTargetPoint(){

        ROS_INFO_STREAM("BIZ_COMMAND: DELIVER TO HOUSE " << nextHouse);


        geometry_msgs::Point target;
        target.x = houses[nextHouse][0];
        target.y = houses[nextHouse][1];
        target.z = houses[nextHouse][2];

        bizTargetPub.publish(target);

        commandComplete = false;


    }

    void returnToVan(){
        // 1 means return to van
        ROS_INFO("BIZ_COMMAND: RETURN TO VAN");
        std_msgs::Int8 msg;
        msg.data = 2;
        changeStatePub.publish(msg);
        commandComplete = false;

    }

    void commandCompleteCallback(std_msgs::Bool const & msg){

        if (msg.data){
            commandComplete = true;
            return;
        }
        else{
            ROS_ERROR("BIZ_NODE: RECEIVED COMMAND NOT COMPLETE ERROR");
            return;
        }

    }

    void waitForCompletion(){
        
        ROS_INFO("bizNode: waiting for completion");
        ros::Rate rate(5); 
        while(!commandComplete){
            ros::spinOnce();
            rate.sleep();
        }

        ROS_INFO("bizNode: command complete");
    }

    

    public: 
    explicit BusinessNode(ros::NodeHandle &nh){
        ROS_INFO("Inside BizNode Launch");

        loadHouses(nh);

        // stateChangePub = nh.subscribe("/changeState", 1, &StateMachine::receiveState, this);

        ros::Publisher motorArmPub = nh.advertise<std_msgs::Bool>("/firefly/arm",1);
        bizCommandCompleteSub = nh.subscribe("/bizCommandComplete",1, &BusinessNode::commandCompleteCallback, this);

        // this publishes a desired point to get to
        bizTargetPub = nh.advertise<geometry_msgs::Point>("/bizTargetPoint", 1);
        changeStatePub = nh.advertise<std_msgs::Int8>("/changeState", 1);

        // wait for initial hover
        waitForCompletion();
        ROS_INFO("BIZ NODE: COMPLETED WAITING FOR INITIAL HOVER");

        // go to van once
        returnToVan();
        waitForCompletion();

        // now start routine of houce and back
        for (int i=0; i<numHouses; i++){
            
            publishNextTargetPoint();
            waitForCompletion();

            returnToVan();
            waitForCompletion();
            nextHouse += 1;

        }

        ROS_INFO("BIZ NODE: COMPLETED ALL DELIVERIES!!");
    
        // disarmMotors(){
        std_msgs::Bool msg;
        msg.data = false;
        motorArmPub.publish(msg);
    


        return;

        

    }

};

int main(int argc, char** argv){

    ros::init(argc, argv, "businessNode");
    ros::NodeHandle nh;

    ROS_INFO("Starting Business Node");
    BusinessNode biznode(nh);


    ros::spin();
    
    return 0;

}