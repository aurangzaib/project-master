#include "stdafx.h"
#include "header.h"
#include "basic.cpp"
#include "pointers.cpp"
#include "classes.cpp"
#include "classes-pointers.cpp"
#include "classes-operator-overload.cpp"
#include "bottle-horiz-vert-detection.cpp"
#include "bottle-cap-detection.cpp"
#include "sobel-canny-threshold.cpp"
#include "discrete-fourier-transform.cpp"

string GetCWD(void) {
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	string cwd(buff);
	return cwd;
}

string masterproject::prjdir = GetCWD();

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
    BottleDetection bottle("/Meeting-7/original-4.bmp");
    bottle.computeResults();

    // dft on the image
//	discreteFourierTransform
//		DFTInstance0("/Meeting-4/bg-black-white-stripes-horizontal"),
//		DFTInstance1("/Meeting-4/bg-black-white-stripes");
//
//    DFTInstance0.performDFT();
//	DFTInstance1.performDFT();
    //string line;
    //
    //ifstream myfile (masterproject::prjdir + "/Meeting-7/teach-1-maxima.txt");
    //ofstream outfile (masterproject::prjdir + "/Meeting-7/teach-1-maxima-parsed-cpp.txt",ofstream::binary);
    //std::vector<string> arr;
    //
    //if (myfile.is_open()) {
    //    for (size_t loopVar=0; getline(myfile,line); loopVar++) {
    //        arr.push_back(line.substr(0, line.find("*")));
    //   }
    //    for(size_t i = 0; i < arr.size(); ++i) {
    //        outfile << arr[i] << endl; // I also tried replacing endl with a "\n"
    //    }
    //    myfile.close();
    //}
    //
    //else cout << "Unable to open file";
    waitKey();
}