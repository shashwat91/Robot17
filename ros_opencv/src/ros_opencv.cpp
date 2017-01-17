
#include <stdio.h>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

void chatterCallback(const sensor_msgs::ImageConstPtr& image)
{
   //ROS_INFO("I heard: [%s]", msg->data.c_str());
   cv::NamedWindow("window1");
   imshow("ROS Image", image);
   waitKey(0);
}


int main()
{

ros::init(argc, argv, "listener");
ros::NodeHandle n;

ros::Subscriber sub = n.subscribe("/camera/image", 1, chatterCallback);
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
 cv::DestroyWindow( "window1" );
return 0;
}
