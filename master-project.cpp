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
  for (int loop = 1; loop <= 31; loop++) {
    // get image in sequence
    Mat inputImage =
        imread(masterproject::cwd + "meeting-12-2/" + to_string(loop) + ".bmp");
    Size s1 = inputImage.size();

    BottleDetection detectBottles(inputImage);
    CapDetection detectCaps(inputImage, 40, 55);
    // remove the edges of the image before applying the algorithms
    detectBottles.getRegionOfInterest(inputImage,      // image
                               s1.width / 20,   // remove 1/5.2th from left
                               s1.height / 10,  // remove 1/10th from top
                               s1.width - (2 * s1.width / 20),
                               s1.height - s1.height / 5);

    // detect caps of the bottle
    detectCaps.applyHoughCircleTransform();

    // detect presence of the bottle
    detectBottles.performBlobDetection();

    imshow("result", inputImage);
    waitKey(1.5 * 1000);
  }

  // videoFrames(masterproject::cwd + "/meeting-5/cap-teach-video.mp4");
  return 0;
}
