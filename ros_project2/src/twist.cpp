#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

#include <sstream>
#include <ncurses.h>

int kbhit(void)
{
    int ch = getch();
    if (ch != ERR) 
    {
        ungetch(ch);
        return 1;
    }
    else
        return 0;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "test_node2");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1000);
  ros::Rate loop_rate(1);
  
  initscr();

  noecho();
  cbreak();
  nodelay(stdscr, TRUE);
  float val=0;
  int kb_val;
  int count=0;

  while (ros::ok())
  {
    geometry_msgs::Twist xyz;

    if (kbhit()) 
    {
    	kb_val = getch();
    	ROS_INFO("Entered character :: %d", kb_val);
    	refresh();
    } 
    else
    	kb_val = 0;

    if(count%2 == 0)
    {
    	xyz.linear.x = 2.0;
    	xyz.angular.z = 0;
    }
    else 
    {
    	xyz.linear.x = 0;
    	xyz.angular.z = 1.2;
    }
    //ROS_INFO("Entered character :: %d", c);
    chatter_pub.publish(xyz);
    ros::spinOnce();
    loop_rate.sleep();
    count++;
  }
}
