// cblobtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "highgui.h"
#include "cv.h"
#include "BlobResult.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int i;
double area[21];
double perimeter[4];
double length[4];
CvRect rect[21];

IplImage *srcImg =  cvLoadImage("blue.jpg",0);
IplImage *threshold = cvCreateImage(cvGetSize(srcImg),IPL_DEPTH_8U, 1);

cvNamedWindow("original", 1);
//cvNamedWindow("threshold", 1);
cvNamedWindow("blobbed", 1);

CBlobResult blobs;
CBlob *currentBlob;
CBlobGetLength *newblob;

cvShowImage("original",srcImg);

cvThreshold(srcImg, threshold,100.0,255.0,CV_THRESH_OTSU);
//cvShowImage("threshold",threshold);

blobs = CBlobResult(threshold, NULL, 255);
blobs.Filter(blobs,B_INCLUDE,CBlobGetArea(),B_GREATER, 30);
int num_blobs = blobs.GetNumBlobs();

IplImage *filtered = cvCreateImage(cvGetSize(threshold), 8, 3);
cvMerge(threshold, threshold, threshold,NULL, filtered);
//printf("%d\n",num_blobs);
for (i=0; i<num_blobs; i++)
	{
	currentBlob = blobs.GetBlob(i);
	//area[i]=currentBlob->Area();
	rect[i]=currentBlob->GetBoundingBox();
	//perimeter[i]=currentBlob->Perimeter();
	//length[i]=newblob(currentBlob);
	//printf("perimeter=%lf length=%lf\n",perimeter[i],length[i]);
	currentBlob->FillBlob(filtered, CV_RGB(0,255,0));
	cvRectangle(filtered,cvPoint(rect[i].x,rect[i].y),cvPoint(rect[i].x+rect[i].width,rect[i].y+rect[i].height),CV_RGB(0,0,255),2,8,0);
	//printf("%d %d %d %d\n",rect[i].x,rect[i].y,rect[i].width,rect[i].height);
	cvCircle(filtered,cvPoint(rect[i].x+(rect[i].width)/2,rect[i].y+(rect[i].height)/2), 10, CV_RGB(200,50,50),1,8,0);
	//printf("%lf\n",area[i]);
	//printf("%d %d\n",rect[i].x+(rect[i].width)/2,rect[i].y+(rect[i].height)/2);
	}

cvShowImage("blobbed", filtered);
cvWaitKey();
cvReleaseImage(&srcImg);
cvReleaseImage(&threshold);
cvReleaseImage(&filtered);
cvDestroyWindow("original");
cvDestroyWindow("blobbed");
cvDestroyWindow("threshold");
return 0;
}

