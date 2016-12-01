#include "stdafx.h"
#include "header.h"

void laneDetection () {
    
    // input image
    Mat img  = imread(masterproject::prjdir + "/Meeting-4/road-0.jpg", CV_LOAD_IMAGE_COLOR);
    
    // grayscale conversion
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY); // src, output, option
    
    // median blur to reduce the noise
    Mat median;
    medianBlur(gray, median, 5); // src, output, aperture size (should be odd and >1)
    // canny contour detection
    Mat canny;
    Canny(median, canny, 50,255, 3); // src, output, low thresh, high thresh, kernel
    
    // thesholding
    Mat thresh;
    threshold(canny, thresh, 80, 255, THRESH_BINARY_INV);
    
    imshow("original image: ", img);
    imshow( "gray image", gray);
    imshow( "median blur image", median);
    imshow( "canny contour image", canny);
    imshow( "threshold image", thresh);
    
    waitKey ();
}

