#include "stdafx.h"
#include "header.h"
#include "basic.cpp"
#include "pointers.cpp"
#include "classes.cpp"
#include "classes-pointers.cpp"
#include "classes-operator-overload.cpp"
#include "lane-detection.cpp"
#include "bottle-cap-detection.cpp"

// for osx
string masterproject::prjdir = "/Users/aurangzaib/Documents/Hochschule/Winter 2016/Projects/Master Project";
// for windows
// string masterproject::prjdir = "C:/Users/aurangzaib/Documents/Master-Project";

int main() {

#if false
	playWithStrings();
#endif

#if false
	playWithArrays();
#endif

#if false
	playWithFunctions();
#endif

#if false
	playWithPointers();
#endif

#if false
	playWithPointerToPointer();
#endif

#if false
	playWithClass();
#endif

#if false
	playWithClassPointer();
#endif

#if false
	playWithClassOverloading();
#endif

#if false
	sobelCannyThreshold();
#endif

#if false
	bottleCapDetection();
#endif

#if true
	laneDetection();
#endif

	return 0;
}