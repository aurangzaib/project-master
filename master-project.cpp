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

string masterproject::prjdir =
    "/Users/siddiqui/Documents/Projects/master-project";

int main() {
  BottleDetection bottle("/Meeting-7/original-11.bmp");
  bottle.computeResults();
  waitKey();
}
