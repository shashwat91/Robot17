// whitedetect.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cv.h" 
 #include "highgui.h" 
 #include <stdio.h>

int _tmain(int argc, _TCHAR* argv[])
{
		
 CvCapture* capture = cvCaptureFromCAM(0);
IplImage* frame;
cvNamedWindow("window1");

int i,j,k,count;
int height,width,step,channels;
int stepr, channelsr;
int temp=0;
uchar *data,*datar;
//cvWaitKey(3000);
count=0;
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
		if((data[i*step+j*channels+1]) > 200 && (data[i*step+j*channels+2]) > 200 && (data[i*step+j*channels]) > 200)
		datar[i*stepr+j*channelsr]=255;
		else
		datar[i*stepr+j*channelsr]=0;
	}
}
cvShowImage("Result",result);
cvWaitKey(1);
count++;
if(count==100)
{
	cvSaveImage("arena.jpg",frame);
	printf("image saved");
}
}
cvReleaseCapture( &capture );
 cvDestroyWindow( "window1" );
 cvDestroyWindow("Result");

	return 0;
}

