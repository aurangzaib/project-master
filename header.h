#ifndef header_h
#define header_h
#include "stdafx.h"
// cpp
#include <iostream>
#include <string>
// opencv
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
// namespace
using namespace std;
using namespace cv;
// declarations
void laneDetection(void);
void bottleCapDetection(void);
void playWithArrays(void);
void playWithPointerToPointer(void);
void sobelCannyThreshold(void);
// project directory - windows
// string prjdir = "C:/Users/aurangzaib/Documents/Master-Project";
// project directory - osx
string prjdir = "/Users/aurangzaib/Documents/Hochschule/Winter 2016/Projects/Master Project";

#endif /* header_h */
