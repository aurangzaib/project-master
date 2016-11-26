#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

void sobelCannyThreshold () {
    
    string prjdir = "/Users/aurangzaib/Documents/Hochschule/Winter 2016/Projects/Master Project/";
    Mat src, grey, sobelx, canny, thresholdImage, draw;
    double minValue, maxValue;
    
    src = imread( prjdir+"/Meeting-2/3-light-vertical.jpg", CV_LOAD_IMAGE_COLOR );
    // reduce noise with 3x3 kernel
    blur(src, src, Size(3, 3)) ;
    // transform image from 1 color system to another
    cvtColor(src,grey, CV_BGR2GRAY);
    // 3x3 sobelx
    Sobel(grey, sobelx, CV_32F, 1, 0, 3);
    // find max and min in the image
    minMaxLoc(grey, &minValue, &maxValue);
    // make sobelx dense, initially its sparse matrix
    sobelx.convertTo(draw, CV_32F);
    // perform thresholding
    threshold(grey, thresholdImage, 80, 255, THRESH_BINARY);
    // perform canny edge detector
    Canny(grey, canny, 0, 150, 3);
    
    // max and min values of grey scale image
    cout << "min value: " << minValue
    << " max value: " << maxValue
    << endl ;
    
    // show images
    imshow( "Original image", src);
    imshow( "greyscale image", grey );
    imshow( "sobel x image", sobelx);
    imshow( "canny edge image", canny);
    imshow( "threshold image", thresholdImage);
    
    // write to disk
    imwrite( prjdir+"/Meeting-4/profilepic-grey.bmp", grey); // save as grey image
    imwrite( prjdir+"/Meeting-4/profilepic-sobel.bmp", sobelx); // save as grey image
    imwrite( prjdir+"/Meeting-4/profilepic-canny.bmp", canny); // save as grey image
    
    // keep window open until enter is pressed
    waitKey (0);
}