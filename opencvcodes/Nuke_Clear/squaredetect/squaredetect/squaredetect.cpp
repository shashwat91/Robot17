// squaredetect.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "highgui.h"
#include "cv.h"
#include "BlobResult.h"


int _tmain(int argc, _TCHAR* argv[])
{
	int i;
	CvSeq* contours;
    CvSeq* result;
    CvMemStorage *storage = cvCreateMemStorage(0);
IplImage *img=  cvLoadImage("try.jpg",0);
IplImage *threshold = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U, 1);
cvThreshold(img, threshold,100.0,255.0,CV_THRESH_OTSU);
//IplImage *temp=cvCreateImage(cvGetSize(threshold), 8, 1);
//cvCvtColor(img, threshold, CV_BGR2GRAY);
cvNamedWindow("original", 1);
cvNamedWindow("blobbed", 1);

cvFindContours(threshold, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
 while(contours)
    {
        result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0);
		if(result->total==4  && fabs(cvContourArea(result, CV_WHOLE_SEQ)) > 20)
        {	printf("square detected\n");
            CvPoint *pt[4];
            for(i=0;i<4;i++)
                pt[i] = (CvPoint*)cvGetSeqElem(result, i);
 
            cvLine(threshold, *pt[0], *pt[1], cvScalar(255));
            cvLine(threshold, *pt[1], *pt[2], cvScalar(255));
            cvLine(threshold, *pt[2], *pt[3], cvScalar(255));
            cvLine(threshold, *pt[3], *pt[0], cvScalar(255));
        }
		 contours = contours->h_next;
    }
 cvShowImage("original",img);
 

cvShowImage("blobbed", threshold);
cvWaitKey();
 
cvReleaseImage(&threshold);
//cvReleaseImage(&temp);
cvReleaseMemStorage(&storage);
cvReleaseImage(&img);
cvDestroyWindow("original");
cvDestroyWindow("blobbed");
return 0;
}

