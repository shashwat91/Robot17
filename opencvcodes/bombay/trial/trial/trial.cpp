#include "stdafx.h"
#include "cv.h" 
 #include "highgui.h"
#include "BlobResult.h"
 #include <stdio.h>
#include<math.h>

int _tmain(int argc, _TCHAR* argv[])
{	CvCapture* capture = cvCaptureFromCAM(2);
IplImage* frame;
cvNamedWindow("window1");
cvNamedWindow("Blue");
//cvNamedWindow("Red");
//cvNamedWindow("crop");
int i,j,k;
int height,width,step,channels;
int stepr, channelsr,stepb, channelsb;
int numball=0,temp=0;
int send=0;
uchar *data;
uchar *datar;
uchar *datab;

while(1)
 {
frame = cvQueryFrame( capture );
//cvSetImageROI(original, cvRect(10, 116,508,255));
//IplImage *frame=cvCreateImage( cvGetSize(original),original->depth,original->nChannels );
//cvCopy(original,frame, NULL);
//cvResetImageROI(original);

IplImage *red=cvCreateImage( cvGetSize(frame), 8, 1 );
stepr=red->widthStep;
channelsr=red->nChannels;
datar = (uchar *)red->imageData;

IplImage *blue=cvCreateImage( cvGetSize(frame), 8, 1 );
stepb=blue->widthStep;
channelsb=blue->nChannels;
datab = (uchar *)blue->imageData;

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
		if(((data[i*step+j*channels+2]) > (50+data[i*step+j*channels]))
		&& ((data[i*step+j*channels+2]) > (50+data[i*step+j*channels+1])))
		{datar[i*stepr+j*channelsr]=255;
		 numball++;
		}
		else
		datar[i*stepr+j*channelsr]=0;

		
		if(((data[i*step+j*channels]) > (30+data[i*step+j*channels+2]))
		&& ((data[i*step+j*channels]) > (30+data[i*step+j*channels+1])))
		datab[i*stepb+j*channelsb]=0;
		else
		datab[i*stepb+j*channelsb]=255;

	}
}
//printf("%d\n",numball);
CvMoments momentsr;
cvMoments(red, &momentsr, 1);
double m00r, m10r, m01r;
float xred,yred;
m00r = cvGetSpatialMoment(&momentsr, 0,0);
m10r = cvGetSpatialMoment(&momentsr, 1,0);
m01r = cvGetSpatialMoment(&momentsr, 0,1);
// TBD check that m00 != 0
if (m00r != 0)
{
xred = m10r/m00r;
yred = m01r/m00r;

//cvCircle(frame,cvPoint(int(xred),int(yred)), 10, cvScalar(0,0,0),2,8,0);
}
double area[2];
CvRect rect[2];
int frontx,fronty,backx,backy;
float anglef,angleb;
CBlobResult blobs;
CBlob *currentBlob;
blobs = CBlobResult(blue, NULL, 255);
blobs.Filter(blobs,B_INCLUDE,CBlobGetArea(),B_GREATER, 40);
int num_blobs = blobs.GetNumBlobs();
//printf("%d\n",num_blobs);
for (i=0; i<num_blobs; i++) 
{
	currentBlob = blobs.GetBlob(i);
	area[i]=currentBlob->Area();
	rect[i]=currentBlob->GetBoundingBox();
	cvRectangle(frame,cvPoint(rect[i].x,rect[i].y),cvPoint(rect[i].x+rect[i].width,rect[i].y+rect[i].height),CV_RGB(0,0,255),2,8,0);
	cvCircle(frame,cvPoint(rect[i].x+(rect[i].width)/2,rect[i].y+(rect[i].height)/2), 10,cvScalar(0,255,0),2,8,0);
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
//printf("%f %f\n",xred,yred);
//printf("%d\n",numball);
printf("%f %f \n",area[0],area[1]);
//cvCircle(blue,cvPoint(int(frontx),int(fronty)), 10, cvScalar(0,0,0),2,8,0);
//cvCircle(blue,cvPoint(int(backx),int(backy)), 10, cvScalar(0,0,0),2,8,0);
anglef=atan((fronty-yred)/(frontx-xred));
angleb=atan((backy-yred)/(backx-xred));
cvShowImage("window1",frame);
//cvShowImage("Red",red);
cvShowImage("Blue",blue);
//cvShowImage("crop",frame);
cvWaitKey(1);
}
cvReleaseCapture( &capture );
 cvDestroyWindow( "window1" );
 //cvDestroyWindow("Red");
 cvDestroyWindow("Blue");
return 0;
}