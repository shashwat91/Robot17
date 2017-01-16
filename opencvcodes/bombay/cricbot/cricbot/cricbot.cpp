// cricbot.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cv.h" 
 #include "highgui.h"
#include "BlobResult.h"
 #include <stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
void wait ( int seconds )
{
  clock_t endwait;
  endwait = clock () + seconds * CLOCKS_PER_SEC ;
  while (clock() < endwait) {}
}


int main()
{
	
CvCapture* capture = cvCaptureFromCAM(1);
IplImage* original;
cvNamedWindow("window1");

int i,j,k;
int height,width,step,channels;
int stepr, channelsr,stepb, channelsb;
int numball,temp=0;
int send=0;
uchar *data;
uchar *datar;
uchar *datab;
double area[2];
CvRect rect[2];
int frontx,fronty,backx,backy,centerx,centery;
float anglef,angleb,anglec;
double m00r, m10r, m01r;
float xred,yred;
int num_blobs=0,flag=0;
float distf,distb;
int blackx=367,blacky=41;


CvMoments momentsr;
CBlobResult blobs;
system( "MODE COM1: BAUD=1200 PARITY=n DATA=8 STOP=1" ) ;
FILE* port;

while(1)
 {
original = cvQueryFrame( capture );
cvSetImageROI(original, cvRect(10, 116,508,255));
IplImage *frame=cvCreateImage( cvGetSize(original),original->depth,original->nChannels );
cvCopy(original,frame, NULL);
cvResetImageROI(original);

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

i=j=k=numball=0;
for(i=0;i < (height);i++) 
{
	for(j=0;j <(width);j++)
	{
		if(((data[i*step+j*channels+2]) > (40+data[i*step+j*channels]))
		&& ((data[i*step+j*channels+2]) > (40+data[i*step+j*channels+1])))
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

cvMoments(red, &momentsr, 1);
m00r = cvGetSpatialMoment(&momentsr, 0,0);
m10r = cvGetSpatialMoment(&momentsr, 1,0);
m01r = cvGetSpatialMoment(&momentsr, 0,1);
if (m00r != 0)
{
xred = m10r/m00r;
yred = m01r/m00r;
}

CBlob *currentBlob;
blobs = CBlobResult(blue, NULL, 255);
blobs.Filter(blobs,B_INCLUDE,CBlobGetArea(),B_GREATER, 10);
num_blobs = blobs.GetNumBlobs();
for (i=0; i<num_blobs; i++) 
{
	currentBlob = blobs.GetBlob(i);
	area[i]=currentBlob->Area();
	rect[i]=currentBlob->GetBoundingBox();
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
anglef=atan((yred-fronty)/(xred-frontx));
angleb=atan((yred-backy)/(xred-backx));
anglec=atan((yred-centery)/(xred-centerx));
distf=sqrt(((xred-frontx)*(xred-frontx))+((yred-fronty)*(yred-fronty)));
distb=sqrt(((xred-backx)*(xred-backx))+((yred-backy)*(yred-backy)));
if((numball>100)&&(flag!=2))
{
	if(distf<45)
	flag=1;
	else
	flag=0;

	if((angleb>(anglec+0.05))&&(anglef<(anglec-0.05)))
	{
		if(send!=2)
		{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
		}
		send=2;
	}
	else if(anglef>(anglec+0.05) && angleb<(anglec-0.05))
	{
		if(send!=3)
		{
		port = fopen( "COM1:", "w" ) ;
		fprintf( port, "r" ) ;
		fclose( port ) ;
		}
		send=3;
	}
	else
	{
		if((send!=1)&&(distb>distf))
		{
		port = fopen( "COM1:", "w" ) ;
		fprintf( port, "f" ) ;
		fclose( port ) ;
		send=1;
		}
		else if(distb<distf)
		{
			if(send!=3)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "r" ) ;
			fclose( port ) ;
			}
		}
		

	}
}
else if(flag==1)
{
	port = fopen( "COM1:", "w" ) ;
	fprintf( port, "s" ) ;
	fclose( port ) ;
	send=4;
	port = fopen( "COM1:", "w" ) ;
	fprintf( port, "d" ) ;
	fclose( port ) ;
	wait(1.5);
	flag=2;
}
else if(flag==2)
{
	xred=blackx;
	yred=blacky;
	anglef=atan((yred-fronty)/(xred-frontx));
	angleb=atan((yred-backy)/(xred-backx));
	anglec=atan((yred-centery)/(xred-centerx));
	distf=sqrt(((xred-frontx)*(xred-frontx))+((yred-fronty)*(yred-fronty)));
	distb=sqrt(((xred-backx)*(xred-backx))+((yred-backy)*(yred-backy)));
	if(distf>10)
	{
	if((angleb>(anglec+0.05))&&(anglef<(anglec-0.05)))
	{
		if(send!=2)
		{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
		}
		send=2;
	}	
	else if(anglef>(anglec+0.05) && angleb<(anglec-0.05))
	{
		if(send!=3)
		{
		port = fopen( "COM1:", "w" ) ;
		fprintf( port, "r" ) ;
		fclose( port ) ;
		}
		send=3;
	}
	else if(anglec>anglef && anglef>angleb)
	{
		if(send!=3)
		{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
		}
		send=3;
	}
	else
	{
		if((send!=6)&&(distb>distf))
		{
		port = fopen( "COM1:", "w" ) ;
		fprintf( port, "f" ) ;
		fclose( port ) ;
		send=6;
		}
		else if(distb<distf)
		{
		if(send!=3)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "r" ) ;
			fclose( port ) ;
			}
		}
		
	}
	}
	else
	{
	if(send!=4)
		{
		port = fopen( "COM1:", "w" ) ;
		fprintf( port, "s" ) ;
		fclose( port ) ;
		port = fopen( "COM1:", "w" ) ;
		fprintf( port, "u" ) ;
		fclose( port ) ;
		wait(3);
		flag=0;
		}
		send=4;


	}
}

else
{
		if(send!=4)
		{
		port = fopen( "COM1:", "w" ) ;
		fprintf( port, "s" ) ;
		fclose( port ) ;
		}
		send=4;
}




cvShowImage("window1",frame);

cvReleaseImage(&blue);
cvReleaseImage(&red);
cvWaitKey(1);
}
cvReleaseCapture( &capture );
 cvDestroyWindow( "window1" );

	return 0;
}

