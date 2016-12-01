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
string masterproject::prjdir = "/Users/aurangzaib/Documents/Hochschule/Winter 2016/Projects/Master Project";
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
    discreteFourierTransform DFTInstance;
    DFTInstance.performDFT();
    DFTInstance.showDFTResult();
    
    return 0;
}