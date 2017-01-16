// imtool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include<time.h>
void wait ( int seconds )
{
  clock_t endwait;
  endwait = clock () + seconds * CLOCKS_PER_SEC ;
  while (clock() < endwait) {}
}
void mouseHandler(int event, int x, int y, int flags, void* param)
{
    IplImage* img0, * img1;
    CvFont    font;
    uchar*    ptr;
    char      label[20];
 
    img0 = (IplImage*) param;
    img1 = cvCloneImage(img0);
 
    cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, .8, .8, 0, 1, 8);
 
    if (event == CV_EVENT_LBUTTONDOWN)
    {
        /* read pixel */
		//sprintf(label,"%d %d\n",cvPoint.x,cvPoint.y);
        ptr = cvPtr2D(img1, y, x, NULL);
 
        /*
         * display the BGR value
         */
        
		sprintf(label, "(%d, %d, %d)", ptr[0], ptr[1], ptr[2]);
		cvRectangle(
            img1,
            cvPoint(x, y - 12),
            cvPoint(x + 100, y + 4),
            CV_RGB(255, 0, 0),
            CV_FILLED,
            8, 0
        );
 
        cvPutText(
            img1,
            label,
            cvPoint(x, y),
            &font,
            CV_RGB(255, 255, 0)
        );
 
        cvShowImage("img", img1);
		//wait(1);
    }
}

int main(int argc, char** argv)
{
 CvCapture* capture = cvCaptureFromCAM(1);
	IplImage* img;
 cvNamedWindow("img", 1);
 wait(3);
    /* usage: <prog_name> <image> 
    if (argc != 2) {
        printf("Usage: %s <image>\n", argv[0]);
        return 1;
    }*/
 while(1)
 {
    /* load image */
    img = cvQueryFrame( capture );
 
    /* always check */
    //assert(img);
 
      
    cvSetMouseCallback("img", mouseHandler, (void*)img);
 
    cvShowImage("img", img);
 
    cvWaitKey(1);
 }
    cvReleaseCapture( &capture );
    cvDestroyAllWindows();
    
    return 0;
}
