
#include <stdio.h>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#define width 405
#define height 720

void chatterCallback(const sensor_msgs::ImageConstPtr& msg)
{
   //ROS_INFO("I heard: [%s]", msg->data.c_str());
   int x,y;
   cv_bridge::CvImagePtr img_ptr;
   cv::Mat rgb_img,flip_img, small,canny,lines;
   cv::namedWindow("window1");
   cv::namedWindow("window2");
   //cv::namedWindow("window3");
   img_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8);
   rgb_img=img_ptr ->image;
   cvtColor(rgb_img,rgb_img,CV_BGR2GRAY);
   
   
   cv::transpose(rgb_img, rgb_img);
   cv::flip(rgb_img,flip_img,1);
   cv::resize(flip_img,small,cvSize(width,height));
   cv::Mat crop_img = small(cv::Rect(10,200,width-21,height-201));
   
   //cv::vector<Vec2f> lines;
  //threshold( rgb_img, th, 180 , 255,1);	//Setting thrashhold
   cv::Canny(crop_img, canny, 20, 100, 3);	//Performing canny  //50,200,3
   //cv::Canny(crop_img, canny1, 150, 200, 3);
  // cv::HoughLinesP(canny, lines, 1, 0.01, 10, 50, 100);	//Detecting lines
   /* Perform edge detection. */
	//cv::toCanny(rgb_img, rgb_img);   

	/* Perform line detection. */
	//cv::toHough(rgb_img, lines); 
  /* cv::Moments moments();//canny,true);
   double m00, m10, m01;

   m00 = moments.m00;//cvGetSpatialMoment(&moments, 0,0);
   m10 = moments.m10;//cvGetSpatialMoment(&moments, 1,0);
   m01 = moments.m01;//cvGetSpatialMoment(&moments, 0,1);
   cv::Point center;
// TBD check that m00 != 0
if (m00 != 0) {
center.x = m10/m00;
center.y = m01/m00;
*/
//CODE FOR GETTING THE MOMENTS
  std::vector<std::vector<cv::Point> > contour;
  std::vector<cv::Vec4i> heirarchy;
  cv::findContours(canny, contour, heirarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));
  std::vector<cv::Moments> mu(contour.size());
  std::vector<cv::Point2f> mc(contour.size());
  double ptr=0.0;
  int count=0;
  for(int i = 0; i<contour.size();++i)
{
	mu[i] = cv::moments(contour[i], false);
	mc[i] = cv::Point2f(mu[i].m10/mu[i].m00, mu[i].m01/mu[i].m00);
	if(mc[i].x >0)	
		{
		ptr += mc[i].x;
		count++;
		}
      //  std::cout<<"value :: "<<mc[i].x<<", ::"<<(double)mu[i].m10/(double)mu[i].m00<<"\n";
       // pt[i] = cv::Point2f(mu[i].m10/mu[i].m00, height/2);
	cv::circle(canny,mc[i], 10, cv::Scalar(255,0,0),2,8,0);
}
	if(count !=0)
	        ptr /= count; 
	else 
		ptr=0;
   std::cout<<"value :: "<<ptr<<"\n";
   cv::Point2f cent(ptr,(height-201)/2);
   cv::circle(crop_img,cent, 10, cv::Scalar(255,0,0),2,8,0);
   //cv::imshow("canny", canny); 
   cv::imshow("window1", crop_img);
   cv::imshow("window2", canny); 
 //cv::imshow("window3", canny1); 
   cv::waitKey(1);
}


int main(int argc, char **argv)
{

ros::init(argc, argv, "listener");
ros::NodeHandle n;
image_transport::ImageTransport it(n);

image_transport::Subscriber sub = it.subscribe("/camera/image", 1, chatterCallback);
//CvCapture* capture = cvCaptureFromCAM(0);
/*IplImage* frame;
cvNamedWindow("window1");
while(1)
{
frame = cvQueryFrame( capture );
cvShowImage("window1",frame);
cvWaitKey(1);
//cvSaveImage("image1.png",frame);
}
cvReleaseImage(&frame);
cvReleaseCapture( &capture ); */
ros::spin();
 cv::destroyWindow( "window1" );
return 0;
}
