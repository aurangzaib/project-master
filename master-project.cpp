#include <iostream>
#include <string>
#include "basic.cpp"
#include "pointers.cpp"
#include "classes.cpp"
#include "classes-pointers.cpp"
#include "classes-operator-overload.cpp"
#include "lane-detection.cpp"
#include "stdafx.h"
using namespace std;

void laneDetection(void);
void bottleCapDetection(void);
void playWithArrays(void);
void playWithPointerToPointer(void);
void sobelCannyThreshold(void);

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