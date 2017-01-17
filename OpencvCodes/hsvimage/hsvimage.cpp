#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n"); //give image as argument while running
        return -1;
    }

    Mat image_rgb,image_hsv;
    image_rgb = imread( argv[1], 1 );

    if ( !image_rgb.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("RGB Image", WINDOW_AUTOSIZE );
    imshow("RGB Image", image_rgb);
    cvtColor(image_rgb,image_hsv,  CV_RGB2HSV);
    namedWindow("HSV Image", WINDOW_AUTOSIZE );
    imshow("HSV Image", image_hsv);

    waitKey(0);

    return 0;
}
