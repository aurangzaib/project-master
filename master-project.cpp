#include "stdafx.h"
#include "header.h"
#include "basic.cpp"
#include "pointers.cpp"
#include "classes.cpp"
#include "classes-pointers.cpp"
#include "classes-operator-overload.cpp"
#include "lane-detection.cpp"
#include "bottle-cap-detection.cpp"
#include "sobel-canny-threshold.cpp"
// for osx
string masterproject::prjdir = "/Users/aurangzaib/Documents/Hochschule/Winter 2016/Projects/Master Project";
// for windows
// string masterproject::prjdir = "C:/Users/aurangzaib/Documents/Master-Project";

int main() {

#if true
	playWithStrings();
#endif

#if true
	playWithArrays();
#endif

#if true
	playWithFunctions();
#endif

#if true
	playWithPointers();
#endif

#if true
	playWithPointerToPointer();
#endif

#if true
	playWithClass();
#endif

#if true
	playWithClassPointer();
#endif

#if true
	playWithClassOverloading();
#endif

#if true
	sobelCannyThreshold();
#endif

#if true
	bottleCapDetection();
#endif

#if true
	laneDetection();
#endif

	return 0;
}