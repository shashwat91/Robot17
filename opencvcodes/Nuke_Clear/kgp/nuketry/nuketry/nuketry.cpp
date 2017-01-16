// nuketry.cpp : Defines the entry point for the console application.
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
	
IplImage* arena=cvLoadImage("arena.jpg",1);
cvNamedWindow("window1");
cvNamedWindow("arena");
cvNamedWindow("white");

CvCapture* capture = cvCaptureFromCAM(0);
IplImage* frame;
int i,j,k,x,y,z,a=1,b=8,shift=50;
int trackstartx,trackstarty,min=20,margin=30;
float xrotate,yrotate,startx,starty,endx,endy;
int trackhalf,trackcomplete,xline=0,yline=0;
int height,width,step,channels;
int aheight,awidth,astep,achannels;
int stepr, channelsr,stepb, channelsb,stepw,channelsw,stepf,channelsf;
int temp=0;
int send=0,redbox=0;
uchar *data;
uchar *adata;
uchar *datar;
uchar *datab;
uchar *dataw;
uchar *dataf;
CvPoint redpt[3];
CvPoint box[5];
double area[2];
CvRect rect[2];
int rightx,righty,leftx,lefty,xbot,ybot;
int beginx,beginy,finishx,finishy;
int num_blobs=0,black=0,white1=0;
float distl,distr,trackx,tracky,botdist;
CBlobResult blobs;
system( "MODE COM1: BAUD=1200 PARITY=n DATA=8 STOP=1" ) ;
FILE* port;

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
num_blobs=0;
CBlob *currentBlob;
blobs = CBlobResult(red, NULL, 255);
blobs.Filter(blobs,B_INCLUDE,CBlobGetArea(),B_GREATER, 30);
redbox = blobs.GetNumBlobs();
//printf("%d\n",num_blobs);
for (i=0; i<redbox; i++) 
{
	currentBlob = blobs.GetBlob(i);
	area[i]=currentBlob->Area();
	rect[i]=currentBlob->GetBoundingBox();
	//cvCircle(arena,cvPoint(rect[i].x+(rect[i].width)/2,((rect[i].y+(rect[i].height)/2))), 10,cvScalar(0,255,0),2,8,0);
	redpt[i].x= rect[i].x+(rect[i].width)/2;
	redpt[i].y= rect[1].y+(rect[1].height)/2;
}

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
beginx=xbot;
beginy=ybot;
if(redbox==1)
 {
 box[0].x=redpt[0].x;
 box[0].y=redpt[0].y+shift;
 box[1].x=finishx;
 box[1].y=finishy;
}
 else
 {
	if(redpt[0].x<redpt[1].x)
	{
		box[0].x=redpt[0].x;
		box[0].y=redpt[0].y+shift;
		box[1].x=beginx;
		box[1].y=beginy;
		box[2].x=redpt[1].x;
		box[2].y=redpt[1].y+shift;
		box[3].x=finishx;
		box[3].y=finishy;

 	}
	else
	{
		box[0].x=redpt[1].x;
		box[0].y=redpt[1].y+shift;
		box[2].x=redpt[0].x;
		box[2].y=redpt[0].y+shift;
		box[3].x=finishx;
		box[3].y=finishy;

 	}
 }
//cvCircle(arena,cvPoint(int(box[0].x),int(box[0].y)), 10, cvScalar(255,0,0),2,8,0);
z=0;
while(z<4)
{

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
cvCircle(frame,cvPoint(int(xbot),int(ybot)), 10, cvScalar(0,0,0),2,8,0);
printf("bot=%d %d\n",xbot,ybot);
//cvShowImage("window1",frame);

	trackstartx=xbot;
	trackstarty=ybot;
	if(z==1)
	{
		trackstartx=box[0].x;
		trackstarty=box[0].y;
		xbot=box[0].x;
		ybot=box[0].y;
	}

	xline=0;
	yline=0;
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
	printf("track=%d %d\n",trackx,tracky);
	trackcomplete=0;
	trackhalf=0;
	while(trackcomplete!=1)
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
		
		if(trackhalf==0)
		{
			startx=trackstartx;
			starty=trackstarty;
			endx=trackx;
			endy=tracky;
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
						if(ybot>endy+margin)
						{
							xrotate=xbot;
							yrotate=endy-2*margin;
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
						else if(ybot<endy-margin)
						{
							xrotate=xbot;
							yrotate=endy+2*margin;
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
						if(xbot>endx+margin)
						{
							xrotate=endx-2*margin;
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
						else if(xbot<endx-margin)
						{
							xrotate=endx+2*margin;
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
		}

		else if(trackhalf==1)
		{
			startx=trackx;
			starty=tracky;
			endx=box[z].x;
			endy=box[z].y;
			botdist=sqrt(((endx-xbot)*(endx-xbot))+((endy-ybot)*(endy-ybot)));
			if(botdist<min)
			{
				trackcomplete=1;
				if(send!=4)
				{
				port = fopen( "COM1:", "w" ) ;
				fprintf( port, "t" ) ;
				fclose( port ) ;
				}
				send=4;
				cvWaitKey(3000);
				break;
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
						if(ybot>endy+margin)
						{
							xrotate=xbot;
							yrotate=endy-2*margin;
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
						else if(ybot<endy-margin)
						{
							xrotate=xbot;
							yrotate=endy+2*margin;
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
						if(xbot>endx+margin)
						{
							xrotate=endx-2*margin;
							yrotate=ybot;
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
						else if(xbot<endx-margin)
						{
							xrotate=endx+2*margin;
							yrotate=ybot;
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
		}
	

cvShowImage("window1",frame);

cvReleaseImage(&blue);
cvWaitKey(1);
	}
	z++;
}	
cvReleaseImage(&red);
cvReleaseCapture( &capture );
cvDestroyWindow( "window1" );

return 0;
}

