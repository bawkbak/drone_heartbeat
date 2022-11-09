#include <iostream>
#include <math.h>

#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Bool.h>
using namespace std; 

class Heartbeat{
    private:
        ros::NodeHandle n;
        ros::Publisher pub_path_heartbeat;
        
        ros::Subscriber sub_state;

        bool path_state = false;
    public:
        Heartbeat();
        void pathStateCallback(const std_msgs::Bool::ConstPtr& msg);
};

Heartbeat :: Heartbeat(){
    pub_path_heartbeat = n.advertise<std_msgs::Int32>("follow_path_heartbeat", 10);

    sub_state = n.subscribe<std_msgs::Bool>("path_state", 1,  &Heartbeat::pathStateCallback, this);
}

void Heartbeat :: pathStateCallback(const std_msgs::Bool::ConstPtr& msg){
    std_msgs::Int32 msg_heartbeat;
    if(msg->data == true){
        msg_heartbeat.data = 2;
    }else{
        msg_heartbeat.data = 1;
    }
    pub_path_heartbeat.publish(msg_heartbeat);
    return;
}


int main(int argc, char **argv){
    ros::init(argc, argv, "follow_path_heartbeat");
    Heartbeat follow_path;
    ros::spin();
    return 0;
}