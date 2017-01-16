// regionofinterest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cv.h" 
 #include "highgui.h" 
 #include <stdio.h>


int main()
{
	IplImage* frame= cvLoadImage("test3.jpg");
	cvSetImageROI(frame, cvRect(50, 55, 450, 550));
cvNamedWindow("window1");

IplImage *subimg=cvCreateImage( cvGetSize(frame),  frame->depth,
                               frame->nChannels );
cvCopy(frame, subimg, NULL);
cvResetImageROI(frame);
int i,j,k;
int height,width,step,channels;
int stepr, channelsr;
int temp=0;
uchar *data,*datar;

IplImage *result=cvCreateImage( cvGetSize(subimg),8,1);
cvNamedWindow("Result");

stepr=result->widthStep;
channelsr=result->nChannels;
datar = (uchar *)result->imageData;

height = subimg->height;
width = subimg->width;
step =subimg->widthStep;
channels = subimg->nChannels;
data = (uchar *)subimg->imageData;

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

cvShowImage("window1",frame);
cvShowImage("Result",result);
cvWaitKey();

 cvDestroyWindow( "window1" );
 cvDestroyWindow("Result");

	return 0;
}

