// cvmoments.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cv.h" 
 #include "highgui.h" 
 #include <stdio.h>


int main()
{
CvCapture* capture = cvCaptureFromCAM(1);
IplImage* frame;
cvNamedWindow("window1");

int i,j,k;
int height,width,step,channels;
int stepr, channelsr;
int temp=0;
uchar *data,*datar;

while(1)
{
frame = cvQueryFrame( capture );
cvShowImage("window1",frame);
IplImage *result=cvCreateImage( cvGetSize(frame), 8, 1 );
cvNamedWindow("Result");
stepr=result->widthStep;
channelsr=result->nChannels;
datar = (uchar *)result->imageData;

height = frame->height;
width = frame->width;
step =frame->widthStep;
channels = frame->nChannels;
data = (uchar *)frame->imageData;

i=j=k=0;
for(i=0;i < (height);i++) 
{
	for(j=0;j <(width);j++)
	{
		if(((data[i*step+j*channels+2]) > (60+data[i*step+j*channels]))
		&& ((data[i*step+j*channels+2]) > (60+data[i*step+j*channels+1])))
		datar[i*stepr+j*channelsr]=255;
		else
		datar[i*stepr+j*channelsr]=0;
	}
}

CvMoments moments;
cvMoments(result, &moments, 1);
double m00, m10, m01;

m00 = cvGetSpatialMoment(&moments, 0,0);
m10 = cvGetSpatialMoment(&moments, 1,0);
m01 = cvGetSpatialMoment(&moments, 0,1);

// TBD check that m00 != 0
if (m00 != 0) {
float xcenter = m10/m00;
float ycenter = m01/m00;

cvCircle(result,cvPoint(int(xcenter),int(ycenter)), 10, cvScalar(0,0,0),2,8,0);
}

cvShowImage("Result",result);
cvWaitKey(1);
}
cvReleaseCapture( &capture );
 cvDestroyWindow( "window1" );
 cvDestroyWindow("Result");
 return 0;
}

