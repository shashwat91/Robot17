// calibrate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cv.h" 
 #include "highgui.h"
#include "BlobResult.h"
 #include <stdio.h>
#include<math.h>
#include<stdlib.h>
#define BLOCK 10
int main()
{
CvCapture* capture = cvCaptureFromCAM(0);
IplImage* frame;
IplImage* arena=cvLoadImage("arena.jpg",1);
cvNamedWindow("window1");
cvNamedWindow("arena");
cvNamedWindow("white");
int i,j,k,x,y,z,a=1,b=8,shift=50;
int trackstartx,trackstarty,min=20,margin=30;
float xrotate,yrotate,startx,starty,endx,endy;
int trackhalf,trackcomplete,xline=0,yline=0;
int height,width,step,channels,black=0,white1=0;
int aheight,awidth,astep,achannels;
int stepr, channelsr,stepb, channelsb,stepw,channelsw,stepf,channelsf;
int temp=0;
int send=0,square=0;
uchar *data;
uchar *adata;
uchar *datar;
uchar *datab;
uchar *dataw;
uchar *dataf;
CvPoint redpt[3];
CvPoint box[3];
double area[2];
CvRect rect[2];
int rightx,righty,leftx,lefty,xbot,ybot;
int num_blobs=0;
float distl,distr,trackx,tracky,botdist;
CBlobResult blobs;

height = arena->height;
width = arena->width;
step =arena->widthStep;
channels = arena->nChannels;
data = (uchar *)arena->imageData;
IplImage *red=cvCreateImage( cvGetSize(arena), 8, 1 );
stepr=red->widthStep;
channelsr=red->nChannels;
datar = (uchar *)red->imageData;

IplImage *white=cvCreateImage( cvGetSize(arena), 8, 1 );
stepw=white->widthStep;
channelsw=white->nChannels;
dataw = (uchar *)white->imageData;

y=x=k=0;
for(y=0;y < (height);y++) 
{
	for(x=0;x <(width);x++)
	{
		if(((data[y*step+x*channels+2]) > (60+data[y*step+x*channels]))
		&& ((data[y*step+x*channels+2]) > (60+data[y*step+x*channels+1])))
		datar[y*stepr+x*channelsr]=0;
		else
		datar[y*stepr+x*channelsr]=255;

		if((data[y*step+x*channels+1]) > 150 && (data[y*step+x*channels+2]) > 150 && (data[y*step+x*channels]) > 150)
		dataw[y*stepw+x*channelsw]=255;
		else
		dataw[y*stepw+x*channelsw]=0;
	}
}
IplImage *filter=cvCreateImage( cvGetSize(white), 8, 1 );
stepf=filter->widthStep;
channelsf=filter->nChannels;
dataf = (uchar *)filter->imageData;

y=x=k=0;
for(y=0;y<(height);y+=BLOCK) 
{
	for(x=0;x <(width);x+=BLOCK)
	{
		for(i=y;i<y+BLOCK;i++)
		{
			for(j=x;j<x+BLOCK;j++)
			{
				if(dataw[i*stepw+j*channelsw]==0)
					black++;
				else
					white1++;
			}
		}
	if(black>white1)
	{
    for(i=y;i<y+BLOCK;i++)
		{
			for(j=x;j<x+BLOCK;j++)
			{
			dataf[i*stepw+j*channelsw]=0;
			}
		}
	}
	else
	{
    for(i=y;i<y+BLOCK;i++)
		{
			for(j=x;j<x+BLOCK;j++)
			{
			dataf[i*stepw+j*channelsw]=255;
			}
		}
	}
	black=0;
	white1=0;
  }
}
cvShowImage("white",filter);
num_blobs=0;
CBlob *currentBlob;
blobs = CBlobResult(red, NULL, 255);
blobs.Filter(blobs,B_INCLUDE,CBlobGetArea(),B_GREATER, 30);
num_blobs = blobs.GetNumBlobs();
//printf("%d\n",num_blobs);
for (i=0; i<num_blobs; i++) 
{
	currentBlob = blobs.GetBlob(i);
	area[i]=currentBlob->Area();
	rect[i]=currentBlob->GetBoundingBox();
	//cvCircle(arena,cvPoint(rect[i].x+(rect[i].width)/2,((rect[i].y+(rect[i].height)/2))), 10,cvScalar(0,255,0),2,8,0);
	redpt[i].x= rect[i].x+(rect[i].width)/2;
	redpt[i].y= rect[1].y+(rect[1].height)/2;
}
if(num_blobs==1)
 {
 box[0].x=redpt[0].x;
 box[0].y=redpt[0].y+shift;
 }
 else
 {
	if(redpt[0].x<redpt[1].x)
	{
		box[0].x=redpt[0].x;
		box[0].y=redpt[0].y+shift;
		box[1].x=redpt[1].x;
		box[1].y=redpt[1].y+shift;
 	}
	else
	{
		box[0].x=redpt[1].x;
		box[0].y=redpt[1].y+shift;
		box[1].x=redpt[0].x;
		box[1].y=redpt[0].y+shift;
 	}
 }
cvCircle(arena,cvPoint(int(box[0].x),int(box[0].y)), 10, cvScalar(255,0,0),2,8,0);
for(i=0;i<50;i++)
{
 frame = cvQueryFrame( capture );
cvShowImage("window1",frame);
cvWaitKey(1);
}

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
		if(((adata[i*astep+j*achannels]) > (30+adata[i*astep+j*achannels+2]))
		&& ((adata[i*astep+j*achannels]) > (30+adata[i*astep+j*achannels+1])))
		datab[i*stepb+j*channelsb]=0;
		else
		datab[i*stepb+j*channelsb]=255;
	}
}
num_blobs=0;
//CBlob *currentBlob;
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
printf("bot=%d %d\n",xbot,ybot);
cvRectangle(frame,cvPoint((xbot-margin),(ybot-margin)),cvPoint((xbot+margin),(ybot+margin)),CV_RGB(0,0,255),2,8,0);

	trackstartx=xbot;
	trackstarty=ybot;
	xline=0;yline=0;
	z=0;
	/*if(z==1)
	{
		trackstartx=box[0].x;
		trackstarty=box[0].y;
		xbot=box[0].x;
		ybot=box[0].y;
	}*/	
	while(xline!=1 && yline!=1)
	{
		
		if(ybot<box[z].y)
		{
			while(ybot<box[z].y)
			{
				if(dataf[ybot*stepf+xbot*channelsf]==0)
				{
					yline=0;
					xbot=trackstartx;
					ybot=trackstarty;
					break;
				}
				else
					yline=1;

				ybot++;
			}
		}
		else if(ybot>box[z].y)
		{
			while(ybot>box[z].y)
			{
				if(dataf[ybot*stepf+xbot*channelsf]==0)
				{
					yline=0;
					xbot=trackstartx;
					ybot=trackstarty;
					break;
				}
				else
					yline=1;

				ybot--;
			}
		}
		else
		{
			yline=1;
		}
		
		if(xline!=1 && yline==1)
		{
			trackx=xbot;
			tracky=ybot;
		}
		if(xbot<box[z].x)
		{
			while(xbot<box[z].x)
			{
				if(dataf[ybot*stepf+xbot*channelsf]==0)
				{
					xline=0;
					xbot=trackstartx;
					ybot=trackstarty;
					break;
				}
				else
					xline=1;

				xbot++;
			}
		}
		else if(xbot>box[z].x)
		{
			while(xbot>box[z].x)
			{
				if(dataf[ybot*stepf+xbot*channelsf]==0)
				{
					xline=0;
					xbot=trackstartx;
					ybot=trackstarty;
					break;
				}
				else
					xline=1;

				xbot--;
			}
		}
		else
		{
			xline=1;
		}

		if(xline==1 && yline!=1)
		{
			trackx=xbot;
			tracky=ybot;
		}

	}
	printf("track=%f %f\n",trackx,tracky);
	cvCircle(frame,cvPoint(int(trackx),int(tracky)), 10, cvScalar(0,0,0),2,8,0);
	cvShowImage("window1",frame);
	endx=trackx;
	endy=tracky;
	distl=sqrt(((leftx-endx)*(leftx-endx))+((lefty-endy)*(lefty-endy)));
	distr=sqrt(((rightx-endx)*(rightx-endx))+((righty-endy)*(righty-endy)));
	printf("b=%f\n",distl-distr);
	xrotate=xbot-2*margin;
	yrotate=ybot;
	distl=sqrt(((leftx-xrotate)*(leftx-xrotate))+((lefty-yrotate)*(lefty-yrotate)));
	distr=sqrt(((rightx-xrotate)*(rightx-xrotate))+((righty-yrotate)*(righty-yrotate)));
	printf("a=%f\n",distl-distr);
		




cvShowImage("arena",arena);
//cvShowImage("red",red);
cvWaitKey(0);
cvReleaseImage(&blue);
cvReleaseImage(&red);
cvReleaseCapture( &capture );
cvDestroyWindow( "window1" );

return 0;
}

