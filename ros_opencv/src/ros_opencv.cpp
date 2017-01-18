
#include <stdio.h>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


void chatterCallback(const sensor_msgs::ImageConstPtr& msg)
{
   //ROS_INFO("I heard: [%s]", msg->data.c_str());
   cv_bridge::CvImagePtr img_ptr;
   cv::Mat rgb_img, canny, lines,th;
   cv::namedWindow("window1");
   img_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8);
   rgb_img=img_ptr ->image;
   cvtColor(rgb_img,rgb_img,CV_BGR2GRAY);
	
   threshold( rgb_img, th, 180 , 255,1);	//Setting thrashhold
	cv::Canny(th, canny, 50, 200, 3);	//Performing canny
    cv::HoughLinesP(canny, lines, 1, 0.01, 10, 50, 100);	//Detecting lines
   /* Perform edge detection. */
	//cv::toCanny(rgb_img, rgb_img);   

	/* Perform line detection. */
	//cv::toHough(rgb_img, lines); 
   cv::imshow("ROS Image", rgb_img); 
   cv::imshow("canny", canny); 
   cv::imshow("ROS Image", lines); 
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
