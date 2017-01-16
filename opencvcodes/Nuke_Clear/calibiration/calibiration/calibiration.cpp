// calibiration.cpp : Defines the entry point for the console application.
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
IplImage* arena=cvLoadImage("arena.jpg",1);
cvNamedWindow("window1");
cvNamedWindow("white");
cvNamedWindow("arena");
int i,j,k,x,y,z,shift=80;
int trackstartx,trackstarty,min=10,margin=25;
float xrotate,yrotate,startx,starty,endx,endy;
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
CvPoint redpt[3];
CvPoint box[3];
double area[2];
CvRect rect[2];
int rightx,righty,leftx,lefty,xbot,ybot;
int num_blobs=0,square=0;
float distl,distr,trackx,tracky,botdist,a=1,b=2;
CBlobResult blobs;

//system( "MODE COM1: BAUD=1200 PARITY=n DATA=8 STOP=1" ) ;
FILE* port;

height = arena->height;
width = arena->width;
step =arena->widthStep;
channels = arena->nChannels;
data = (uchar *)arena->imageData;
//capturing red 
IplImage *red=cvCreateImage( cvGetSize(arena), 8, 1 );
stepr=red->widthStep;
channelsr=red->nChannels;
datar = (uchar *)red->imageData;
//capturing white
IplImage *white=cvCreateImage( cvGetSize(arena), 8, 1 );
stepw=white->widthStep;
channelsw=white->nChannels;
dataw = (uchar *)white->imageData;
//intialising with arena
y=x=k=0;
for(y=0;y < (height);y++) 
{
	for(x=0;x <(width);x++)
	{
		if(((data[y*step+x*channels+2]) > (10+data[y*step+x*channels]))
		&& ((data[y*step+x*channels+2]) > (10+data[y*step+x*channels+1])))
		datar[y*stepr+x*channelsr]=255;
		else
		datar[y*stepr+x*channelsr]=0;

		if((data[y*step+x*channels+1]) > 150 && (data[y*step+x*channels+2]) > 150 && (data[y*step+x*channels]) > 150)
		dataw[y*stepw+x*channelsw]=255;
		else
		dataw[y*stepw+x*channelsw]=0;
	}
}
cvShowImage("white",white);
//square detection code
CvSeq* contours;
CvSeq* result;
CvMemStorage *storage = cvCreateMemStorage(0);
j=0;
cvFindContours(red, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
 while(contours)
    {
        result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0);
		if(result->total==4  && fabs(cvContourArea(result, CV_WHOLE_SEQ)) > 30)
        {	printf("square detected\n");
			square++;
            CvPoint *pt[4];
			for(i=0;i<4;i++){
                pt[i] = (CvPoint*)cvGetSeqElem(result, i);
			}
			redpt[j].x= (pt[0]->x + pt[1]->x + pt[2]->x +pt[3]->x)/4;
			redpt[j].y= (pt[0]->y + pt[1]->y + pt[2]->y +pt[3]->y)/4;
			/*if(dataw[redpt[j].y*stepw+(redpt[j].x-shift)*channelsw]==255)
			redpt[j].x=redpt[j].x-shift;
			else if(dataw[redpt[j].y*stepw+(redpt[j].x+shift)*channelsw]==255)
			redpt[j].x=redpt[j].x-shift;
			else if(dataw[(redpt[j].y+shift)*stepw+redpt[j].x*channelsw]==255)
			redpt[j].y=redpt[j].y+shift;
			else if(dataw[(redpt[j].y-shift)*stepw+redpt[j].x*channelsw]==255)
			redpt[j].y=redpt[j].y-shift;*/
			j++;
        }
		 contours = contours->h_next;
    }
 //print center of red square
 if(square==1)
 {
 box[0].x=redpt[0].x;
 box[0].y=redpt[0].y;
 }
 else
 {
	if(redpt[0].y>redpt[1].y)
	{
		box[0].x=redpt[0].x;
		box[0].y=redpt[0].y;
		box[1].x=redpt[1].x;
		box[1].y=redpt[1].y;
 	}
	else
	{
		box[0].x=redpt[1].x;
		box[0].y=redpt[1].y;
		box[1].x=redpt[0].x;
		box[1].y=redpt[0].y;
 	}
 }
cvCircle(arena,cvPoint(int(box[0].x),int(box[0].y-100)), 10, cvScalar(255,0,0),2,8,0);

cvShowImage("arena",red);
/*

//waiting for 50 images
for(i=0;i<50;i++)
{
 frame = cvQueryFrame( capture );
cvShowImage("window1",frame);
cvWaitKey(1);
}
//getting green image
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
//green detection
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
//taking two rectangles
num_blobs=0;
CBlob *currentBlob;
blobs = CBlobResult(blue, NULL, 255);
blobs.Filter(blobs,B_INCLUDE,CBlobGetArea(),B_GREATER, 30);
num_blobs = blobs.GetNumBlobs();
printf("%d\n",num_blobs);
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
cvRectangle(frame,cvPoint((xbot-margin),(ybot-margin)),cvPoint((xbot+margin),(ybot+margin)),CV_RGB(0,0,255),2,8,0);
printf("bot=%d %d\n",xbot,ybot);
cvShowImage("window1",frame);
//getting middle point of track
	trackstartx=xbot;
	trackstarty=ybot;
	xline=0;yline=0;
	z=0;
	if(z==1)
	{
		trackstartx=box[0].x;
		trackstarty=box[0].y;
		xbot=box[0].x;
		ybot=box[0].y;
	}	
	while(xline!=1 && yline!=1)
	{
		
		if(ybot<box[z].y)
		{
			while(ybot<box[z].y)
			{
				if(dataw[ybot*stepw+xbot*channelsw]==0)
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
				if(dataw[ybot*stepw+xbot*channelsw]==0)
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
				if(dataw[ybot*stepw+xbot*channelsw]==0)
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
				if(dataw[ybot*stepw+xbot*channelsw]==0)
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

	frame = cvQueryFrame( capture );
		blue=cvCreateImage( cvGetSize(frame), 8, 1 );
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
							



cvReleaseImage(&blue);
*/cvWaitKey();
cvReleaseImage(&red);
cvReleaseCapture( &capture );
cvDestroyWindow( "window1" );

return 0;
}