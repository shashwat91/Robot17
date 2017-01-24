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
  ros::Publisher chatter_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
  ros::Rate loop_rate(5);
  
  initscr();

  noecho();
  cbreak();
  nodelay(stdscr, TRUE);
  float val=0;
  int kb_val;
  int count=0;

  while (ros::ok())
  {
    bool update = false;
    geometry_msgs::Twist xyz;

    if (kbhit()) 
    {
      kb_val = getch();
      printw("Key pressed :: %c\n",kb_val);
      switch(kb_val)
      {
        case 119:
        case 87:  //Forward, w
            xyz.linear.x = 1.0;
            xyz.angular.z = 0;
            update = true;
            break;
        case 115:
        case 83: //Back, s
            xyz.linear.x = -1.0;
            xyz.angular.z = 0;
            update = true;
            break;
        case 97:
        case 65:  //Left, a
            xyz.linear.x = 0.0;
            xyz.angular.z = 1.0;
            update = true;
            break;
        case 100:
        case 68:  //Right, d
            xyz.linear.x = 0.0;
            xyz.angular.z = -1.0;
            update = true;
            break;
        case 27:
            endwin();
            exit(0);
            break;
      }
      refresh();
    }

    //ROS_INFO("Entered character :: %d", c);
    if(update)
      chatter_pub.publish(xyz);
    ros::spinOnce();
   // loop_rate.sleep();
    //count++;
  }
  endwin();
}
