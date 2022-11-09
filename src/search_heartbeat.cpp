#include <iostream>
#include <string>
#include <math.h>

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <mavros_msgs/State.h>
#include <sensor_msgs/NavSatFix.h>

using namespace std; 

class Heartbeat{
    private:
        ros::NodeHandle n;
        ros::Publisher pub_search_heartbeat;

        ros::Subscriber sub_object;
        ros::Subscriber sub_drone_gps;

        float search_latitude = 0.9487;
        float search_longitude = 0.9487;
        string search_object = "none";
        string msg_report = "";
    public:
        Heartbeat();
        void objectCallback(const std_msgs::String::ConstPtr& msg);
        void droneGPSCallback(const sensor_msgs::NavSatFix::ConstPtr& msg);
};

Heartbeat :: Heartbeat(){
    pub_search_heartbeat = n.advertise<std_msgs::String>("search_heartbeat", 10);
    sub_object = n.subscribe<std_msgs::String>("object", 1,  &Heartbeat::objectCallback, this);
    sub_drone_gps = n.subscribe<sensor_msgs::NavSatFix>("gps", 1,  &Heartbeat::droneGPSCallback, this);
}

void Heartbeat :: objectCallback(const std_msgs::String::ConstPtr& msg){
    if(search_object != msg->data){
        search_object = msg->data;
        if(msg_report.length()){
            msg_report.append(",");
        }
        msg_report.append(search_object);
        msg_report.append(",");
        msg_report.append(to_string(search_latitude));
        msg_report.append(",");
        msg_report.append("S");
        msg_report.append(",");
        msg_report.append(to_string(search_longitude));
        msg_report.append(",");
        msg_report.append("E");
        // cout << msg_report << endl;
        
        
    }
    std_msgs::String msg_search;
    msg_search.data = msg_report;
    pub_search_heartbeat.publish(msg_search);
    return;
}

void Heartbeat :: droneGPSCallback(const sensor_msgs::NavSatFix::ConstPtr& msg){
    search_latitude = msg->latitude;
    search_longitude = msg->longitude;
    return;
}

int main(int argc, char **argv){
    ros::init(argc, argv, "search_heartbeat");
    Heartbeat search;
    ros::spin();
    return 0;
}