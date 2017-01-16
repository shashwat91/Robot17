// nuke1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "cv.h" 
 #include "highgui.h"
#include "BlobResult.h"
 #include <stdio.h>
#include<math.h>
#include<stdlib.h>

int main()
{
	
CvCapture* capture = cvCaptureFromCAM(1);
IplImage* frame;
cvNamedWindow("window1");

int i,j,k;
int height,width,step,channels;
int stepr, channelsr,stepb, channelsb;
int numball,temp=0;
int send=0;
uchar *data;
uchar *datar;
uchar *datab;
double area[2],areared[3];
CvRect rect[2];
CvRect red[2];
int frontx,fronty,backx,backy,centerx,centery;
float xred,yred;
int num_red=0,num_blobs=0;
float distf,distb;

CBlobResult blobs;
system( "MODE COM1: BAUD=1200 PARITY=n DATA=8 STOP=1" ) ;
FILE* port;

while(1)
 {
frame = cvQueryFrame( capture );
IplImage *red=cvCreateImage( cvGetSize(frame), 8, 1 );
//cvNamedWindow("Red");
stepr=red->widthStep;
channelsr=red->nChannels;
datar = (uchar *)red->imageData;

IplImage *blue=cvCreateImage( cvGetSize(frame), 8, 1 );
//cvNamedWindow("Blue");
stepb=blue->widthStep;
channelsb=blue->nChannels;
datab = (uchar *)blue->imageData;

height = frame->height;
width = frame->width;
step =frame->widthStep;
channels = frame->nChannels;
data = (uchar *)frame->imageData;

i=j=k=numball=0;
for(i=0;i < (height);i++) 
{
	for(j=0;j <(width);j++)
	{
		if(((data[i*step+j*channels+2]) > (80+data[i*step+j*channels]))
		&& ((data[i*step+j*channels+2]) > (80+data[i*step+j*channels+1])))
		{datar[i*stepr+j*channelsr]=255;
		 numball++;
		}
		else
		datar[i*stepr+j*channelsr]=0;

		
		if(((data[i*step+j*channels]) > (60+data[i*step+j*channels+2]))
		&& ((data[i*step+j*channels]) > (60+data[i*step+j*channels+1])))
		datab[i*stepb+j*channelsb]=0;
		else
		datab[i*stepb+j*channelsb]=255;

	}
}

CBlob *currentBlob;
blobs = CBlobResult(blue, NULL, 255);
blobs.Filter(blobs,B_INCLUDE,CBlobGetArea(),B_GREATER, 30);
num_blobs = blobs.GetNumBlobs();
//printf("%d\n",num_blobs);
for (i=0; i<num_blobs; i++) 
{
	currentBlob = blobs.GetBlob(i);
	area[i]=currentBlob->Area();
	rect[i]=currentBlob->GetBoundingBox();
	//cvRectangle(frame,cvPoint(rect[i].x,rect[i].y),cvPoint(rect[i].x+rect[i].width,rect[i].y+rect[i].height),CV_RGB(0,0,255),2,8,0);
	//cvCircle(frame,cvPoint(rect[i].x+(rect[i].width)/2,rect[i].y+(rect[i].height)/2), 10,cvScalar(0,255,0),2,8,0);
}
if(area[0]>area[1])
{
	frontx=rect[1].x+(rect[1].width)/2;
	fronty=rect[1].y+(rect[1].height)/2;
	backx=rect[0].x+(rect[0].width)/2;
	backy=rect[0].y+(rect[0].height)/2;
}
else
{
	frontx=rect[0].x+(rect[0].width)/2;
	fronty=rect[0].y+(rect[0].height)/2;
	backx=rect[1].x+(rect[1].width)/2;
	backy=rect[1].y+(rect[1].height)/2;
}
centerx=(frontx+backx)/2;
centery=(fronty+backy)/2;
//cvCircle(frame,cvPoint(frontx,fronty), 10, cvScalar(0,0,0),2,8,0);
//cvCircle(frame,cvPoint(backx,backy), 10, cvScalar(0,0,0),2,8,0);
distf=sqrt(((xred-frontx)*(xred-frontx))+((yred-fronty)*(yred-fronty)));
distb=sqrt(((xred-backx)*(xred-backx))+((yred-backy)*(yred-backy)));

blobs = CBlobResult(red, NULL, 255);
blobs.Filter(blobs,B_INCLUDE,CBlobGetArea(),B_GREATER, 30);
num_red = blobs.GetNumBlobs();
//printf("%d\n",num_blobs);
for (i=0; i<num_red; i++) 
{
	currentBlob = blobs.GetBlob(i);
	areared[i]=currentBlob->Area();
	red[i]=currentBlob->GetBoundingBox();
	//cvCircle(frame,cvPoint(red[i].x+(red[i].width)/2,red[i].y+(red[i].height)/2), 10,cvScalar(0,255,0),2,8,0);
}
cvShowImage("window1",frame);
//cvShowImage("Red",red);
//cvShowImage("Blue",blue);

cvReleaseImage(&blue);
cvReleaseImage(&red);
cvWaitKey(1);
}
cvReleaseCapture( &capture );
 cvDestroyWindow( "window1" );

return 0;


}