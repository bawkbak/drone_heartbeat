#include <iostream>
#include <math.h>

#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Bool.h>
using namespace std; 

class Heartbeat{
    private:
        ros::NodeHandle n;
        ros::Publisher pub_item_heartbeat;
        
        ros::Subscriber sub_item_state;
        ros::Subscriber sub_deliver_state;

        bool deliver_msg = false;
        bool item_msg = false;
    public:
        Heartbeat();
        void itemStateCallback(const std_msgs::Bool::ConstPtr& msg);
        void deliverStateCallback(const std_msgs::Bool::ConstPtr& msg);
        void estimator();
};

Heartbeat :: Heartbeat(){
    pub_item_heartbeat = n.advertise<std_msgs::Int32>("replenishment_heartbeat", 10);

    sub_item_state = n.subscribe<std_msgs::Bool>("item_state", 1,  &Heartbeat::itemStateCallback, this);
    sub_deliver_state = n.subscribe<std_msgs::Bool>("deliver_state", 1,  &Heartbeat::deliverStateCallback, this);
}

void Heartbeat :: itemStateCallback(const std_msgs::Bool::ConstPtr& msg){
    item_msg = msg->data;
    return;
}



void Heartbeat :: deliverStateCallback(const std_msgs::Bool::ConstPtr& msg){
    deliver_msg = msg->data;
    std_msgs::Int32 msg_heartbeat;
    if(deliver_msg == true){
        msg_heartbeat.data = 2;
    }else{
        if(item_msg == true){
            msg_heartbeat.data = 1;
        }else{
            msg_heartbeat.data = 0;
        }
    }
    pub_item_heartbeat.publish(msg_heartbeat);
    return;
}


int main(int argc, char **argv){
    ros::init(argc, argv, "replenishment_heartbeat");
    Heartbeat replenishment;
    ros::spin();
    return 0;
}