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
    
    // videoFrames(masterproject::prjdir + "/meeting-7/position-teach-video.mp4", true,0,0);
    
    // videoFrames(masterproject::prjdir + "/meeting-8/hh/hh-teach-video.mp4", false, 35, 40);
    
    // detection of caps
    
    // videoFrames(masterproject::prjdir + "/meeting-5/cap-teach-video.mp4", false, 30, 35);
    
    // videoFrames(masterproject::prjdir + "/meeting-8/v/v-teach-video.mp4", false, 35, 40);
    
     videoFrames(masterproject::prjdir + "/meeting-8/vh/vh-teach-video.mp4", false, 35, 40);
    
    // videoFrames(masterproject::prjdir + "/meeting-8/vhv/vhv-teach-video.mp4", false, 35, 45);
    
    return 0;
}
