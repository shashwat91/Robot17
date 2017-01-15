#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

#include <sstream>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "test_node2");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
  ros::Rate loop_rate(1);
  float val=0;
  while (ros::ok())
  {
    geometry_msgs::Twist xyz;
    xyz.linear.x = val;
    ROS_INFO("%s :: %f", "Message published with",val);
    chatter_pub.publish(xyz);
    ros::spinOnce();
    loop_rate.sleep();
    val+= 0.5;
  }
}
