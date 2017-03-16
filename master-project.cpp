#include "basic.cpp"
#include "bottle-cap-detection.cpp"
#include "bottle-horiz-vert-detection.cpp"
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

string masterproject::prjdir =
    "/Users/siddiqui/Documents/Projects/master-project";

int main() {

// detection of positions

  //     // with bottle
  //     Mat img1 = imread(masterproject::prjdir +
  //     "/meeting-7/original-11.bmp");
  //     // without bottle
    
       Mat img2 = imread(masterproject::prjdir +
       "/Meetings/meeting-10/no-bottle/8.bmp");

//  BottleDetection detectInstance;
  BottleDetection detectInstance2;

//  detectInstance.computeResults(img1, true);
   detectInstance2.computeResults(img2, true);

  //    videoFrames(masterproject::prjdir +
  //    "/meeting-7/position-teach-video.mp4", true, 0,0);

  //

  //
  //    Mat img1 = imread(masterproject::prjdir  +
  //    "/meeting-9/without-label/projector/with-bottle-1-DFT.png");
  //    Mat img2 = imread(masterproject::prjdir  +
  //    "/meeting-9/without-label/projector/no-bottle-2-DFT.png");
  //    Mat diff;
  //    resize(img1, img1, Size(60, 900), 0,0, INTER_LINEAR);
  //    resize(img2, img2, Size(60, 900), 0,0, INTER_LINEAR);
  //    threshold(img1, img1, 25, 255, THRESH_BINARY);
  //    threshold(img2, img2, 25, 255, THRESH_BINARY);
  //    absdiff(img1, img2, diff);

  //    imshow("difference: ", img1);
  //    imshow("difference2: ", img2);
  //    imshow("difference_1: ", diff);
  waitKey();
  // videoFrames(masterproject::prjdir + "/meeting-8/hh/hh-teach-video.mp4",
  // true, 35, 40);

  // detection of caps

  // videoFrames(masterproject::prjdir + "/meeting-5/cap-teach-video.mp4",
  // false, 30, 35);

  //  videoFrames(masterproject::prjdir + "/meeting-8/v/v-teach-video.mp4",
  //  false, 35, 40);

  // videoFrames(masterproject::prjdir + "/meeting-8/vh/vh-teach-video.mp4",
  // false, 35, 40);

  // videoFrames(masterproject::prjdir + "/meeting-8/vhv/vhv-teach-video.mp4",
  // false, 35, 45);

  return 0;
}
