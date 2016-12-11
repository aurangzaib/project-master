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
#include "discrete-fourier-transform.cpp"
// for osx
string masterproject::prjdir = "C:/Users/aurangzaib/Documents/master-project";
// for windows
// string masterproject::prjdir = "C:/Users/aurangzaib/Documents/Master-Project";

int main() {
//	playWithStrings();
//	playWithArrays();
//	playWithFunctions();
//	playWithPointers();
//	playWithPointerToPointer();
//	playWithClass();
//	playWithClassPointer();
//	playWithClassOverloading();
//	sobelCannyThreshold();
//	bottleCapDetection();
//	laneDetection();

    // dft on the image
	discreteFourierTransform
		DFTInstance0("/Meeting-4/bg-black-white-stripes-horizontal"),
		DFTInstance1("/Meeting-4/bg-black-white-stripes");

    DFTInstance0.performDFT();
	DFTInstance1.performDFT();
    waitKey();
    
	return 0;
}