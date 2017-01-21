//ROS headers
#include <ros/ros.h>

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

void chatterCallback(const sensor_msgs::ImageConstPtr& msg)
{
  cv_bridge::CvImagePtr img_ptr;
  cv::Mat rgb_img,gray_img,flip_img, small,canny,lines;
  
  double left = (width -21)/2 -30;
  double right = (width - 21)/2 + 30;
  double ptr=0.0;
  int count=0;

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

  //CODE FOR GETTING THE MOMENTS
  std::vector<std::vector<cv::Point> > contour;
  std::vector<cv::Vec4i> heirarchy;
  
  cv::findContours(canny, contour, heirarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));
  
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
    cv::circle(canny,mc[i], 10, cv::Scalar(255,0,0),2,8,0);
  }
	
  //Finding center of all moments
  if(count !=0)
	        ptr /= count; 
	else 
		ptr=0;  
  
  cv::Point2f cent(ptr,(height-201)/2);
  
  //Converting calculated center to movement direction
  if(ptr == 0) //Stop
  {
    //No center on image
  }
  else if (ptr > left && ptr< right) //Stright
	{
    cv::circle(crop_img,cent, 10, cv::Scalar(255,0,0),2,8,0);
	}
  else if (ptr <= left) //Left
	{
    cv::circle(crop_img,cent, 20, cv::Scalar(255,0,0),2,8,0);	
	}
  else if (ptr >= right) //Right
	{
    cv::circle(crop_img,cent, 20, cv::Scalar(255,0,0),2,8,0);	
	}

  std::cout<<"value :: "<<ptr<<"\n"; //For debuging
   
  cv::imshow("window1", crop_img); //Grey scale image with weighted center
  cv::imshow("window2", canny); //Edge image with all centers
  cv::waitKey(1);
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  image_transport::ImageTransport it(n);
  image_transport::Subscriber sub = it.subscribe("/camera/image", 1, chatterCallback);
  ros::spin();
  cv::destroyWindow( "window1" );
  return 0;
}
