#ifndef header_h
#define header_h
#include "stdafx.h"
//#define WINDOWS
#ifdef WINDOWS
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
// cpp
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iomanip>
#include <string>
// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
// namespace
using namespace std;
using namespace cv;
// declarations
void playWithArrays(void);
void playWithPointerToPointer(void);
void sobelCannyThreshold(void);
// namespace
namespace masterproject {
    extern string cwd;
    extern string siddiqui;
    extern string umair;
    extern string rizwan;
}
const bool SHOW_IMAGE = false;
#endif /* header_h */
