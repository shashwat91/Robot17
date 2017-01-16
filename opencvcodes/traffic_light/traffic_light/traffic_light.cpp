#include "stdafx.h"
#include "cv.h" 
 #include "highgui.h" 
 #include <stdio.h>

int main()
{
	
 CvCapture* capture = cvCaptureFromCAM(1);
IplImage* frame;
cvNamedWindow("window1");
cvWaitKey(2000);
int i,j,k,pixelcount=0;
int height,width,step,channels;
int stepr, channelsr;
int temp=0;
uchar *data,*datar;
system( "MODE COM1: BAUD=9600 PARITY=n DATA=8 STOP=1" ) ;
FILE* port;
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
		{
		datar[i*stepr+j*channelsr]=255;
		pixelcount++;
		}
		else
		datar[i*stepr+j*channelsr]=0;
	}
}
if(pixelcount>10000)
{ 
	port = fopen( "COM1:", "w" ) ;
	fprintf( port, "1" ) ;
	fclose( port ) ;
	printf("stop\n");
	cvWaitKey(10);
	//cvWaitKey();		
}
else
{
	port = fopen( "COM1:", "w" ) ;
	fprintf( port, "2" ) ;
	fclose( port ) ;
	cvWaitKey(100);
	printf("go\n");
	//cvWaitKey();		
}
pixelcount=0;
cvShowImage("Result",result);
cvWaitKey(1);
}
fcloseall();
cvReleaseCapture( &capture );
 cvDestroyWindow( "window1" );
 cvDestroyWindow("Result");
 return 0;
}

