#include "basic.cpp"
#include "bottle-cap-detection.cpp"
#include "classes-operator-overload.cpp"
#include "discrete-fourier-transform.cpp"
#include "header.h"
#include "stdafx.h"
#include "video-frames.cpp"

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
  bool BY_REFERENCE = true;
  bool SAVE_RESULTS = false;
  vector<Mat> images;
  fetchImagesFromFolder(images, masterproject::cwd + "meeting-12/lower/");
  for (auto& inputImage : images) {
    Size s1 = inputImage.size();
    BottleDetection regionOfInterest(inputImage);
    // remove the edges of the image before applying the algorithms
    regionOfInterest.getRegionOfInterest(
        inputImage,      // image
        s1.width / 10,   // remove 1/5.2th from left
        s1.height / 10,  // remove 1/10th from top
        s1.width - (2 * s1.width / 10), s1.height - s1.height / 5);
    Mat capData, blobData;
    if (BY_REFERENCE == true) {
      capData = inputImage;
      blobData = inputImage;
    } else if(BY_REFERENCE == false) {
      inputImage.copyTo(capData);
      inputImage.copyTo(blobData);
    }

    CapDetection detectCaps(capData, 45, 65);
    BottleDetection detectBottles(blobData);

    // detect caps of the bottle
    detectCaps.applyHoughCircleTransform();
    // detect presence of the bottle
    detectBottles.performBlobDetection();
    if (BY_REFERENCE == true) {
      imshow("results: ", inputImage);
      if (SAVE_RESULTS) ::saveImage(masterproject::cwd + "/meeting-14/results/result.bmp", inputImage);
    } else if (BY_REFERENCE == false) {
      Mat result;
      vconcat(blobData, capData, result);
      hconcat(blobData, capData, result);
      if (SAVE_RESULTS) ::saveImage(masterproject::cwd + "/meeting-14/results/result.bmp", result);
      imshow("results: ", result);
    }
    waitKey();
  }

  return 0;
}
