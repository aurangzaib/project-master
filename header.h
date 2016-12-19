#ifndef header_h
#define header_h
#include "stdafx.h"
#define WINDOWS  /* uncomment this line to use it for windows.*/ 
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
// cpp
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <functional> 
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
