#include "basic.cpp"
#include "bottle-cap-detection.cpp"
#include "classes-operator-overload.cpp"
#include "classes-pointers.cpp"
#include "classes.cpp"
#include "discrete-fourier-transform.cpp"
#include "header.h"
#include "pointers.cpp"
#include "sobel-canny-threshold.cpp"
#include "stdafx.h"
#include "video-frames.cpp"
string GetCWD(void) {
  char buff[FILENAME_MAX];
  GetCurrentDir(buff, FILENAME_MAX);
  string cwd(buff);
  return cwd;
}

string masterproject::cwd =
    "/Users/siddiqui/Documents/Projects/master-project/meetings/";
int main() {
  // CapDetection detectInstance;
  // detectInstance.applyHoughCircleTransform();
  // waitKey();
  videoFrames(masterproject::cwd + "/meeting-5/cap-teach-video.mp4");
  return 0;
}
