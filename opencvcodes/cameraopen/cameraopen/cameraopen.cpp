// cameraopen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cv.h" 
 #include "highgui.h" 
 #include <stdio.h>


int main()
{
CvCapture* capture = cvCaptureFromCAM(0);
IplImage* frame;
cvNamedWindow("window1");
while(1)
{
frame = cvQueryFrame( capture );
cvShowImage("window1",frame);
cvWaitKey(1);
//cvSaveImage("image1.png",frame);
}
cvReleaseImage(&frame);
cvReleaseCapture( &capture );
 cvDestroyWindow( "window1" );
return 0;
}

