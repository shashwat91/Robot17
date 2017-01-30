/**
* Group number : 17
* Student 1:
* Shashwat Verma, 4507746
* Student 2:
* Ishu Goel, 4632699
*/

//ROS headers
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

//OPpenCV headers
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//C++ headers
#include <iostream>
#include <stdio.h>

//Constants
#define width 405
#define height 720
#define lineTolerance 30
geometry_msgs::Twist xyz;  //Global variables for publish message
double prev_ptr=0;

void setPublish(double ptr)
{
  double left = (width -21)/2 - lineTolerance;
  double right = (width - 21)/2 + lineTolerance;

  //Converting calculated center to movement direction
  if(ptr == 0) 		// for blind spot( no line detected)
  {
	if (prev_ptr == 2)		// if last step was turning left, keep turning left
	{
		xyz.linear.x = 0;
    		xyz.angular.z = 1.0;
	}
	else if (prev_ptr ==3)		// if last step was turning right, keep turning right
	{
		xyz.linear.x = 0;
    		xyz.angular.z = -1.0;
	}
	else 				// if last step was moving straight or no movement, do not move
	{
	        xyz.linear.x = 0;
	        xyz.angular.z = 0;
	}
  }
  else if (ptr > left && ptr< right) //Stright
  {
    xyz.linear.x = 1.0;
    xyz.angular.z = 0;
    prev_ptr = 1;
  }
  else if (ptr <= left) //Left
  {
    xyz.linear.x = 0;
    xyz.angular.z = 1.0;
    prev_ptr = 2;
  }
  else if (ptr >= right) //Right
  {
    xyz.linear.x = 0;
    xyz.angular.z = -1.0;
    prev_ptr = 3;
  }
}

void lineDetect(cv::Mat edges)
{
  double ptr=0.0;
  int count=0;

  //CODE FOR GETTING THE MOMENTS
  std::vector<std::vector<cv::Point> > contour;
  std::vector<cv::Vec4i> heirarchy;
  
  cv::findContours(edges, contour, heirarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));
  
  std::vector<cv::Moments> mu(contour.size());
  std::vector<cv::Point2f> mc(contour.size());
  
  //Calculating moments and center of each moments
  for(int i = 0; i<contour.size();++i)
  {
    mu[i] = cv::moments(contour[i], false);
    mc[i] = cv::Point2f(mu[i].m10/mu[i].m00, mu[i].m01/mu[i].m00);
    if(mc[i].x >0)
    {
      ptr += mc[i].x;
      count++;
    }
    cv::circle(edges,mc[i], 10, cv::Scalar(255,0,0),2,8,0);  //All regions are plotted using circles
  }
  
  //Finding center of all moments
  if(count !=0)
          ptr /= count; 
  else 
    ptr=0;

  cv::Point p1(ptr-5,(height-201)/2);
  cv::Point p2(ptr+5,(height-201)/2 + 5);
  cv::rectangle(edges, p1, p2, cv::Scalar(255,0,0), 1, 8, 0);
  
  setPublish(ptr);
  //cv::imshow("window1", crop_img); //Grey scale image with weighted center
  cv::imshow("window2", edges); //Edge image with all centers
}

void ImageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  cv_bridge::CvImagePtr img_ptr;
  cv::Mat rgb_img,gray_img,flip_img, small,canny;
  
  cv::namedWindow("window1");
  cv::namedWindow("window2");
  //cv::namedWindow("window3");
  img_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8);
  rgb_img=img_ptr ->image;
  
  cvtColor(rgb_img,gray_img,CV_BGR2GRAY);
  cv::transpose(gray_img, gray_img);
  cv::flip(gray_img,flip_img,1);
  cv::resize(flip_img,small,cvSize(width,height));
  cv::Mat crop_img = small(cv::Rect(10,200,width-21,height-201));

  /* Perform edge detection. */   
  cv::Canny(crop_img, canny, 20, 100, 3);	//Performing canny  //50,200,3
  lineDetect(canny); //Function to convert edges to direction
  cv::waitKey(1);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "line_algo");
  ros::NodeHandle n;
  ros::Rate loop_rate(5); //Pusblish rate is fixed at 5Hz
  
  image_transport::ImageTransport it(n);
  image_transport::Subscriber sub = it.subscribe("/camera/image", 1, ImageCallback); //Subscribing to decompressed image

  ros::Publisher robo_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1); //Pusblisher on cmd_vel topic

  while(ros::ok())
  {
    robo_pub.publish(xyz);
    ros::spinOnce();
    loop_rate.sleep();
  }
  cv::destroyWindow( "window1" );
  cv::destroyWindow( "window2" );
  return 0;
}
