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

void fetchImagesFromFolder(vector<Mat>& data, const string path) {
  vector<String> fn;
  glob(path, fn, true);  // recurse
  for (size_t k = 0; k < fn.size(); ++k) {
    cv::Mat im = cv::imread(fn[k]);
    if (im.empty()) continue;  // only proceed if sucsessful
    data.push_back(im);
  }
}

int main() {
  vector<Mat> images;
  fetchImagesFromFolder(images, masterproject::cwd + "meeting-14/*.bmp");
  for (auto& inputImage : images) {
    Size s1 = inputImage.size();
    BottleDetection regionOfInterest(inputImage);
    // remove the edges of the image before applying the algorithms
    regionOfInterest.getRegionOfInterest(
        inputImage,      // image
        s1.width / 20,   // remove 1/5.2th from left
        s1.height / 10,  // remove 1/10th from top
        s1.width - (2 * s1.width / 20), s1.height - s1.height / 5);
    Mat capData, bobData;
    inputImage.copyTo(capData);
    inputImage.copyTo(bobData);

    CapDetection detectCaps(capData, 45, 65);
    BottleDetection detectBottles(bobData);

    // detect caps of the bottle
    detectCaps.applyHoughCircleTransform();
    // detect presence of the bottle
    detectBottles.performBlobDetection();
    imshow("horizontal bottles", bobData);
    imshow("vertical bottles", capData);
    waitKey();
  }

  return 0;
}
