#include <iostream>
#include <math.h>

#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <mavros_msgs/State.h>

using namespace std; 

class Heartbeat{
    private:
        ros::NodeHandle n;
        ros::Publisher pub_drone_heartbeat;
        
        ros::Subscriber sub_drone_state;

        int drone_state = 1;
    public:
        Heartbeat();
        void droneStateCallback(const mavros_msgs::State::ConstPtr& msg);
};

Heartbeat :: Heartbeat(){
    pub_drone_heartbeat = n.advertise<std_msgs::Int32>("drone_heartbeat", 10);

    sub_drone_state = n.subscribe<mavros_msgs::State>("drone_state", 1,  &Heartbeat::droneStateCallback, this);
}


void Heartbeat :: droneStateCallback(const mavros_msgs::State::ConstPtr& msg){
    if(msg->mode == (string)"LAND"){
        drone_state = 1;
    }else if(msg->mode == (string)"OFFBOARD"){
        drone_state = 2;
    }else{
        drone_state = 3;
    }
    std_msgs::Int32 msg_heartbeat;
    msg_heartbeat.data = drone_state;
    pub_drone_heartbeat.publish(msg_heartbeat);
    return;
}


int main(int argc, char **argv){
    ros::init(argc, argv, "drone_heartbeat");
    Heartbeat drone;
    ros::spin();
    return 0;
}