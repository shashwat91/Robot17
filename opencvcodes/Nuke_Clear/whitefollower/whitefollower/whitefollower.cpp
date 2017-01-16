// whitefollower.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "math.h"
#include "cv.h" 
 #include "highgui.h" 
#include "BlobResult.h"
#include<stdlib.h>
 #include <stdio.h>

int main()
{
	CvCapture* capture = cvCaptureFromCAM(0);
IplImage* frame;
cvNamedWindow("window1");

int i,x,y,k,send=0;
int height,width,step,channels;
int stepw, channelsw,stepb, channelsb;
int temp=0,num_blobs;
uchar *data,*dataw,*datab;
double area[2];
CvRect rect[2];
int rightx,righty,leftx,lefty,centerx,centery;
int max=30,min=20,a=2;
float distl,distr,trackx,tracky;


CBlobResult blobs;
system( "MODE COM1: BAUD=1200 PARITY=n DATA=8 STOP=1" ) ;
FILE* port;

while(1)
 {
frame = cvQueryFrame( capture );
cvShowImage("window1",frame);
IplImage *white=cvCreateImage( cvGetSize(frame), 8, 1 );
cvNamedWindow("white");

stepw=white->widthStep;
channelsw=white->nChannels;
dataw = (uchar *)white->imageData;

IplImage *blue=cvCreateImage( cvGetSize(frame), 8, 1 );
stepb=blue->widthStep;
channelsb=blue->nChannels;
datab = (uchar *)blue->imageData;

height = frame->height;
width = frame->width;
step =frame->widthStep;
channels = frame->nChannels;
data = (uchar *)frame->imageData;

y=x=k=0;
for(y=0;y < (height);y++) 
{
	for(x=0;x <(width);x++)
	{
		if((data[y*step+x*channels+1]) > 150 && (data[y*step+x*channels+2]) > 150 && (data[y*step+x*channels]) > 150)
		dataw[y*stepw+x*channelsw]=255;
		else
		dataw[y*stepw+x*channelsw]=0;

		if(((data[y*step+x*channels]) > (30+data[y*step+x*channels+2]))	&& ((data[y*step+x*channels]) > (30+data[y*step+x*channels+1])))
		datab[y*stepb+x*channelsb]=0;
		else
		datab[y*stepb+x*channelsb]=255;
	}
}

num_blobs=0;
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
	rightx=rect[1].x+(rect[1].width)/2;
	righty=rect[1].y+(rect[1].height)/2;
	leftx=rect[0].x+(rect[0].width)/2;
	lefty=rect[0].y+(rect[0].height)/2;
}
else
{
	rightx=rect[0].x+(rect[0].width)/2;
	righty=rect[0].y+(rect[0].height)/2;
	leftx=rect[1].x+(rect[1].width)/2;
	lefty=rect[1].y+(rect[1].height)/2;
}
centerx=(rightx+leftx)/2;
centery=(righty+lefty)/2;

if(((dataw[(centery-min)*stepw+centerx*channelsw]==255)&&(dataw[(centery+min)*stepw+centerx*channelsw]==255))&&((dataw[centery*stepw+(centerx-min)*channelsw]==255)&&(dataw[centery*stepw+(centerx+min)*channelsw]==255)))
{	if(send!=1)
		{
		port = fopen( "COM1:", "w" ) ;
		fprintf( port, "f" ) ;
		fclose( port ) ;
		send=1;
		}
}
else if(dataw[(centery-min)*stepw+centerx*channelsw]==0)
{
	if((dataw[centery*stepw+(centerx-max)*channelsw]==255)&&(dataw[centery*stepw+(centerx+max)*channelsw]==255))
	{
		tracky=centery+min;
		trackx=centerx;
		distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
		distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
		if(distl>distr+a)
		{
			if(send!=3)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "r" ) ;
			fclose( port ) ;
			}
			send=3;
		}
		else if(distr>distl+a)
		{
			if(send!=2)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
			}
			send=2;
		}
		else
		{
			if(send!=3)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "r" ) ;
			fclose( port ) ;
			}
			send=3;
		}
	}
	else if((dataw[centery*stepw+(centerx-max)*channelsw]==0)&&(dataw[centery*stepw+(centerx+max)*channelsw]==255))
	{
		tracky=centery;
		trackx=centerx+min;
		distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
		distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
		if(distl>distr+a)
		{
			if(send!=3)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "r" ) ;
			fclose( port ) ;
			}
			send=3;
		}
		else if(distr>distl+a)
		{
			if(send!=2)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
			}
			send=2;
		}
		else
		{
			tracky=centery+min;
			trackx=centerx;
			distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
			distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
			if(distl>distr+a)
			{
				if(send!=3)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "r" ) ;
				fclose( port ) ;
				}
				send=3;
			}
			else if(distr>distl+1)
			{
				if(send!=2)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "l" ) ;
				fclose( port ) ;
				}
				send=2;
			}
		}
	}
	else if((dataw[centery*stepw+(centerx-max)*channelsw]==255)&&(dataw[centery*stepw+(centerx+max)*channelsw]==0))
	{
		tracky=centery;
		trackx=centerx-min;
		distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
		distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
		if(distl>distr+a)
		{
			if(send!=3)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "r" ) ;
			fclose( port ) ;
			}
			send=3;
		}
		else if(distr>distl+a)
		{
			if(send!=2)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
			}
			send=2;
		}
		else
		{
			tracky=centery+min;
			trackx=centerx;
			distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
			distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
			if(distl>distr+a)
			{
				if(send!=3)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "r" ) ;
				fclose( port ) ;
				}
				send=3;
			}
			else if(distr>distl+a)
			{
				if(send!=2)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "l" ) ;
				fclose( port ) ;
				}
				send=2;
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
		}
		send=4;
	}
}
else if(dataw[(centery+min)*stepw+centerx*channelsw]==0)
{
	if((dataw[centery*stepw+(centerx-max)*channelsw]==255)&&(dataw[centery*stepw+(centerx+max)*channelsw]==255))
	{
		tracky=centery-min;
		trackx=centerx;
		distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
		distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
		if(distl>distr+a)
		{
			if(send!=3)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "r" ) ;
			fclose( port ) ;
			}
			send=3;
		}
		else if(distr>distl+a)
		{
			if(send!=2)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
			}
			send=2;
		}
		else
		{
			if(send!=2)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
			}
			send=2;
		}
	}
	else if((dataw[centery*stepw+(centerx-max)*channelsw]==0)&&(dataw[centery*stepw+(centerx+max)*channelsw]==255))
	{
		tracky=centery;
		trackx=centerx+min;
		distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
		distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
		if(distl>distr+1)
		{
			if(send!=3)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "r" ) ;
			fclose( port ) ;
			}
			send=3;
		}
		else if(distr>distl+a)
		{
			if(send!=2)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
			}
			send=2;
		}
		else
		{
			tracky=centery-min;
			trackx=centerx;
			distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
			distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
			if(distl>distr+a)
			{
				if(send!=3)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "r" ) ;
				fclose( port ) ;
				}
				send=3;
			}
			else if(distr>distl+a)
			{
				if(send!=2)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "l" ) ;
				fclose( port ) ;
				}
				send=2;
			}
		}
	}
	else if((dataw[centery*stepw+(centerx-max)*channelsw]==255)&&(dataw[centery*stepw+(centerx+max)*channelsw]==0))
	{
		tracky=centery;
		trackx=centerx-min;
		distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
		distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
		if(distl>distr+a)
		{
			if(send!=3)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "r" ) ;
			fclose( port ) ;
			}
			send=3;
		}
		else if(distr>distl+a)
		{
			if(send!=2)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
			}
			send=2;
		}
		else
		{
			tracky=centery-min;
			trackx=centerx;
			distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
			distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
			if(distl>distr+a)
			{
				if(send!=3)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "r" ) ;
				fclose( port ) ;
				}
				send=3;
			}
			else if(distr>distl+a)
			{
				if(send!=2)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "l" ) ;
				fclose( port ) ;
				}
				send=2;
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
		}
		send=4;
	}
}

else if(dataw[centery*stepw+(centerx-min)*channelsw]==0)
{
	if((dataw[(centery-max)*stepw+centerx*channelsw]==255)&&(dataw[(centery+max)*stepw+centerx*channelsw]==255))
	{
		tracky=centery;
		trackx=centerx+min;
		distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
		distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
		if(distl>distr+a)
		{
			if(send!=3)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "r" ) ;
			fclose( port ) ;
			}
			send=3;
		}
		else if(distr>distl+a)
		{
			if(send!=2)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
			}
			send=2;
		}
		else
		{
			if(send!=2)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
			}
			send=2;
	
		}
	}
	else if((dataw[(centery-max)*stepw+centerx*channelsw]==0)&&(dataw[(centery+max)*stepw+centerx*channelsw]==255))
	{
		tracky=centery+min;
		trackx=centerx;
		distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
		distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
		if(distl>distr+a)
		{
			if(send!=3)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "r" ) ;
			fclose( port ) ;
			}
			send=3;
		}
		else if(distr>distl+a)
		{
			if(send!=2)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
			}
			send=2;
		}
		else
		{
			tracky=centery;
			trackx=centerx+min;
			distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
			distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
			if(distl>distr+a)
			{
				if(send!=3)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "r" ) ;
				fclose( port ) ;
				}
				send=3;
			}
			else if(distr>distl+a)
			{
				if(send!=2)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "l" ) ;
				fclose( port ) ;
				}
				send=2;
			}
		}
	}
	else if((dataw[(centery-max)*stepw+centerx*channelsw]==255)&&(dataw[(centery+max)*stepw+centerx*channelsw]==0))
	{
		tracky=centery-min;
		trackx=centerx;
		distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
		distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
		if(distl>distr+a)
		{
			if(send!=3)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "r" ) ;
			fclose( port ) ;
			}
			send=3;
		}
		else if(distr>distl+a)
		{
			if(send!=2)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
			}
			send=2;
		}
		else
		{
			tracky=centery;
			trackx=centerx+min;
			distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
			distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
			if(distl>distr+a)
			{
				if(send!=3)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "r" ) ;
				fclose( port ) ;
				}
				send=3;
			}
			else if(distr>distl+a)
			{
				if(send!=2)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "l" ) ;
				fclose( port ) ;
				}
				send=2;
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
		}
		send=4;
	}
}
else if(dataw[centery*stepw+(centerx+min)*channelsw]==0)
{
	if((dataw[(centery-max)*stepw+centerx*channelsw]==255)&&(dataw[(centery+max)*stepw+centerx*channelsw]==255))
	{
		tracky=centery;
		trackx=centerx-min;
		distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
		distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
		if(distl>distr+1)
		{
			if(send!=3)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "r" ) ;
			fclose( port ) ;
			}
			send=3;
		}
		else if(distr>distl+a)
		{
			if(send!=2)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
			}
			send=2;
		}
		else
		{
			if(send!=3)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "r" ) ;
			fclose( port ) ;
			}
			send=3;	
	
		}
	}
	else if((dataw[(centery-max)*stepw+centerx*channelsw]==0)&&(dataw[(centery+max)*stepw+centerx*channelsw]==255))
	{
		tracky=centery+min;
		trackx=centerx;
		distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
		distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
		if(distl>distr+a)
		{
			if(send!=3)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "r" ) ;
			fclose( port ) ;
			}
			send=3;
		}
		else if(distr>distl+1)
		{
			if(send!=2)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
			}
			send=2;
		}
		else
		{
			tracky=centery;
			trackx=centerx-min;
			distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
			distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
			if(distl>distr+a)
			{
				if(send!=3)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "r" ) ;
				fclose( port ) ;
				}
				send=3;
			}
			else if(distr>distl+a)
			{
				if(send!=2)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "l" ) ;
				fclose( port ) ;
				}
				send=2;
			}
		}
	}
	else if((dataw[(centery-max)*stepw+centerx*channelsw]==255)&&(dataw[(centery+max)*stepw+centerx*channelsw]==0))
	{
		tracky=centery-min;
		trackx=centerx;
		distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
		distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
		if(distl>distr+a)
		{
			if(send!=3)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "r" ) ;
			fclose( port ) ;
			}
			send=3;
		}
		else if(distr>distl+a)
		{
			if(send!=2)
			{
			port = fopen( "COM1:", "w" ) ;
			fprintf( port, "l" ) ;
			fclose( port ) ;
			}
			send=2;
		}
		else
		{
			tracky=centery;
			trackx=centerx-min;
			distl=sqrt(((leftx-trackx)*(leftx-trackx))+((lefty-tracky)*(lefty-tracky)));
			distr=sqrt(((rightx-trackx)*(rightx-trackx))+((righty-tracky)*(righty-tracky)));
			if(distl>distr+a)
			{
				if(send!=3)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "r" ) ;
				fclose( port ) ;
				}
				send=3;
			}
			else if(distr>distl+a)
			{
				if(send!=2)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "l" ) ;
				fclose( port ) ;
				}
				send=2;
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
cvShowImage("white",white);
cvReleaseImage(&white);
cvWaitKey(1);
}
cvReleaseCapture( &capture );
 cvDestroyWindow( "window1" );
 cvDestroyWindow("white");
 return 0;
}

