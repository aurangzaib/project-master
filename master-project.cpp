#include "cap_detection.cpp"
#include "header.h"

string prj::siddiqui =
    "/Users/siddiqui/Documents/Projects/master-project/meetings/";
string prj::umair = "/Users/Umair/Desktop/master-project/meetings/";
string prj::rizwan = "Undefined (No User Found)";
string prj::cwd = prj::siddiqui;

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
  fetchImagesFromFolder(images, prj::cwd + "meeting-15/");
  for (auto& inputImage : images) {
    Size s1 = inputImage.size();
    BottleDetection regionOfInterest(inputImage);
    // remove the edges of the image before applying the algorithms
    regionOfInterest.getRegionOfInterest(
        inputImage,      // image
        s1.width / 7,    // remove 1/7th from left
        s1.height / 10,  // remove 1/10th from top
        s1.width - (2 * s1.width / 7), s1.height - s1.height / 4);
    Mat capData, blobData;
    if (BY_REFERENCE == true) {
      capData = inputImage;
      blobData = inputImage;
    } else if (BY_REFERENCE == false) {
      inputImage.copyTo(capData);
      inputImage.copyTo(blobData);
    }
    // instance of cap detection class
    CapDetection detectCaps(capData, 45, 65);
    // instance of bottle detection class
    BottleDetection detectBottles(blobData);
    // detect caps of the bottle
    detectCaps.getCaps();
    // detect presence of the bottle
    detectBottles.getBottles();
    // detect dark bottles
    if (NO_FILTER) detectBottles.getDarkBottles();
    // use images by address
    if (BY_REFERENCE == true) {
      if (ENLARGE_RESULTS)
        resize(inputImage, inputImage, Size(), 2.3, 2.3, INTER_LINEAR);
      if (SHOW_RESULT) imshow("results: ", inputImage);
      if (SAVE_RESULTS)
        ::saveImage(prj::cwd + "/results/result.bmp", inputImage);
    }
    // use images by copy
    else if (BY_REFERENCE == false) {
      Mat result;
      vconcat(blobData, capData, result);
      hconcat(blobData, capData, result);
      if (ENLARGE_RESULTS)
        resize(inputImage, inputImage, Size(), 2.3, 2.3, INTER_LINEAR);
      if (SAVE_RESULTS)
        ::saveImage(prj::cwd + "/meeting-15/results/result.bmp", result);
      if (SHOW_RESULT) imshow("results: ", result);
    }
    // keep images showing
    waitKey(350);
  }
  return 0;
}
