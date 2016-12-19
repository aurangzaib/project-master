#ifndef header_h
#define header_h
#include "stdafx.h"
// cpp
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
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
void bottleHorizVertDetection(void);
void bottleCapDetection(void);
void playWithArrays(void);
void playWithPointerToPointer(void);
void sobelCannyThreshold(void);
// namespace
namespace masterproject {
    extern string prjdir;
}

#endif /* header_h */
