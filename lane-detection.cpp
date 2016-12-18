#include "stdafx.h"
#include "header.h"

void laneDetection () {
    
    // http://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/hough_lines/hough_lines.html
    
    // input image
    Mat img  = imread(masterproject::prjdir + "/Meeting-4/road-0.jpg", CV_LOAD_IMAGE_COLOR);
    
    // grayscale conversion
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY); // src, output, option
    
    // median blur to reduce the noise
    Mat median;
    medianBlur(
               gray,    // source
               median,  // destination
               3        // aperture size (odd and >1)
               );
    
    // canny contour detection
    Mat canny;
    Canny(
          median,   // source
          canny,    // destination
          120,       // low threshold
          200,      // high threshold
          3         // kernel size 3x3
          );
    
    // thesholding
    Mat thresh;
    threshold(
              canny,
              thresh,
              80,
              255,
              THRESH_BINARY_INV
              );
    
    // to save lines from hough transform
    vector <Vec2f> lines;
    
    // it will generate the accumulator cell
    // which is a 2xn matric contaning r->firstrow and theta->secondrow
    HoughLines(
               canny,           // source
               lines,           // destination
               1,               // rho resolution
               CV_PI/180,       // theta resolution (1 degree here)
               120,             // min # of intersection to detect a line
               0,0              // srn and stn
               );
    
    Mat detection;
    img.copyTo (detection);
    for (size_t loopVar=0; loopVar<lines.size(); loopVar++ ) {
        
        // hough coordinates [row, theta]
        float rho = lines[loopVar][0]; // first row
        float theta = lines[loopVar][1]; // second row
        
        // cartesian coordinates [x, y]
        Point pt1, pt2;
        
        double a = cos (theta), b = sin (theta);
        
        double x0 = rho*a; // r * cos(theta)
        double y0 = rho*b; // r * sin(thera)
        
        // point 1
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        
        // point 2
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        
        // draw line using point1 and point2. red color
        line(detection, pt1, pt2, Scalar(0,0,255), 1, CV_AA);
    }
    
    imshow("1-original image: ", img);
    imshow("2-gray image", gray);
    imshow("3-median blur image", median);
    imshow("4-canny contour image", canny);
    imshow("5-threshold image", thresh);
    imshow("6-hough line transform ", detection);
    
    waitKey ();

}
