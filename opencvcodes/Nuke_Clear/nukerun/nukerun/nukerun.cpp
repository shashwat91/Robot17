// nukerun.cpp : Defines the entry point for the console application.
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
IplImage* arena=cvLoadImage("arena.jpg",0);
cvNamedWindow("window1");

int i,j,k,x,y,z,a=1,b=2,shift=80;
int trackstartx,trackstarty,min=10,margin=35;
float xrotate,yrotate,startx=394,starty=418,endx=394,endy=186;
int trackhalf,trackcomplete,xline=0,yline=0;
int height,width,step,channels;
int aheight,awidth,astep,achannels;
int stepr, channelsr,stepb, channelsb,stepw,channelsw;
int temp=0;
int send=0;
uchar *data;
uchar *adata;
uchar *datar;
uchar *datab;
uchar *dataw;
CvPoint *redpt[3];
CvPoint *box[3];
double area[2];
CvRect rect[2];
int rightx,righty,leftx,lefty,xbot,ybot;
int num_blobs=0;
float distl,distr,trackx,tracky,botdist;
CBlobResult blobs;
system( "MODE COM1: BAUD=1200 PARITY=n DATA=8 STOP=1" ) ;
FILE* port;

height = arena->height;
width = arena->width;
step =arena->widthStep;
channels = arena->nChannels;
data = (uchar *)arena->imageData;


IplImage *white=cvCreateImage( cvGetSize(arena), 8, 1 );
stepw=white->widthStep;
channelsw=white->nChannels;
dataw = (uchar *)white->imageData;

y=x=k=0;
for(y=0;y < (height);y++) 
{
	for(x=0;x <(width);x++)
	{
		if((data[y*step+x*channels+1]) > 200 && (data[y*step+x*channels+2]) > 200 && (data[y*step+x*channels]) > 200)
		dataw[y*stepw+x*channelsw]=255;
		else
		dataw[y*stepw+x*channelsw]=0;
	}
}
for(i=0;i<50;i++)
{
 frame = cvQueryFrame( capture );
cvShowImage("window1",frame);
cvWaitKey(1);
}
trackhalf=0;
while(trackhalf!=1)
{

frame = cvQueryFrame( capture );
IplImage *blue=cvCreateImage( cvGetSize(frame), 8, 1 );
//cvNamedWindow("Blue");
stepb=blue->widthStep;
channelsb=blue->nChannels;
datab = (uchar *)blue->imageData;

aheight = frame->height;
awidth = frame->width;
astep =frame->widthStep;
achannels = frame->nChannels;
adata = (uchar *)frame->imageData;

i=j=k=0;
for(i=0;i < (aheight);i++) 
{
	for(j=0;j <(awidth);j++)
	{
		if(((adata[i*astep+j*achannels+1]) > (30+adata[i*astep+j*achannels+2]))
		&& ((adata[i*astep+j*achannels+1]) > (7+adata[i*astep+j*achannels])))
		datab[i*stepb+j*channelsb]=0;
		else
		datab[i*stepb+j*channelsb]=255;

	}
}
num_blobs=0;
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
xbot=(rightx+leftx)/2;
ybot=(righty+lefty)/2;

			botdist=sqrt(((endx-xbot)*(endx-xbot))+((endy-ybot)*(endy-ybot)));
			if(botdist<min)
			{
				trackhalf=1;
				if(send!=4)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "s" ) ;
				fclose( port ) ;
				}
				send=4;
			}
			else
			{
				distl=sqrt(((leftx-endx)*(leftx-endx))+((lefty-endy)*(lefty-endy)));
				distr=sqrt(((rightx-endx)*(rightx-endx))+((righty-endy)*(righty-endy)));
				if(distl>distr+b)
				{
					if(send!=3)
					{
					port = fopen( "COM1:", "w" ) ;
					fprintf( port, "r" ) ;
					fclose( port ) ;
					}
					send=3;
				}
				else if(distr>distl+b)
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
					if(starty==endy)
					{
						if(ybot>starty+margin)
						{
							xrotate=xbot;
							yrotate=starty-2*margin;
							distl=sqrt(((leftx-xrotate)*(leftx-xrotate))+((lefty-yrotate)*(lefty-yrotate)));
							distr=sqrt(((rightx-xrotate)*(rightx-xrotate))+((righty-yrotate)*(righty-yrotate)));
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
						else if(ybot<starty-margin)
						{
							xrotate=xbot;
							yrotate=starty+2*margin;
							distl=sqrt(((leftx-xrotate)*(leftx-xrotate))+((lefty-yrotate)*(lefty-yrotate)));
							distr=sqrt(((rightx-xrotate)*(rightx-xrotate))+((righty-yrotate)*(righty-yrotate)));
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
						else
						{
								if(send!=1)
								{
								port = fopen( "COM1:", "w" ) ;
								fprintf( port, "f" ) ;
								fclose( port ) ;
								}
								send=1;
						}
					}
					else if(startx==endx)
					{
						if(xbot>startx+margin)
						{
							xrotate=xbot-2*margin;
							yrotate=starty;
							distl=sqrt(((leftx-xrotate)*(leftx-xrotate))+((lefty-yrotate)*(lefty-yrotate)));
							distr=sqrt(((rightx-xrotate)*(rightx-xrotate))+((righty-yrotate)*(righty-yrotate)));
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
						else if(xbot<startx-margin)
						{
							xrotate=xbot+2*margin;
							yrotate=starty;
							distl=sqrt(((leftx-xrotate)*(leftx-xrotate))+((lefty-yrotate)*(lefty-yrotate)));
							distr=sqrt(((rightx-xrotate)*(rightx-xrotate))+((righty-yrotate)*(righty-yrotate)));
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
						else
						{
								if(send!=1)
								{
								port = fopen( "COM1:", "w" ) ;
								fprintf( port, "f" ) ;
								fclose( port ) ;
								}
								send=1;
						}
					}
				}					
			}
cvShowImage("window1",frame);
cvWaitKey(1);

}
	return 0;
}

