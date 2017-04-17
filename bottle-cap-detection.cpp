#include "bottle-horiz-vert-detection.cpp"
#include "header.h"
#include "stdafx.h"


struct CapNoFilter {
  const float minConvex = 0.65;
  const int minThresholdValue = 10;
  const int filterKernelSize = 3;
  const int markerSize = 1;
} capNoFilterVariables;

struct CapWithFilter {
  const float minConvex = 0.90;
  const int minThresholdValue = 5;
  const int filterKernelSize = 49;
  const int markerSize = 1;
} capWithFilterVariables;

class CapDetection {
    
 private:
  string imagePath;
  unsigned minRadius, maxRadius;

 public:
  Mat inputImage, outputImage;
  CapDetection();
  CapDetection(const string, unsigned, unsigned);
  CapDetection(Mat&, unsigned, unsigned);
  void reduceImageDensity();
  void applyHoughCircleTransform();
  // helper methods
  void getBottlesCircles();
  void getCapsUsingHough();
  void getCapsUsingBlobs();
};

CapDetection::CapDetection() {
  string imagePath = masterproject::cwd + "meeting-5/cap-teach-5.bmp";
  inputImage = imread(imagePath);
  minRadius = 15;
  maxRadius = 35;
}

CapDetection::CapDetection(const string imagePath,  // path to the image
                           unsigned minRadius,      // min radius for circle
                           unsigned maxRadius)      // max radius for circle
    : imagePath(imagePath),
      minRadius(minRadius),
      maxRadius(maxRadius) {
  inputImage = imread(imagePath);
}

CapDetection::CapDetection(Mat& inputImage,     // image as reference
                           unsigned minRadius,  // min radius for circle
                           unsigned maxRadius)  // max radius for circle
    : inputImage(inputImage),
      minRadius(minRadius),
      maxRadius(maxRadius) {
  imagePath = "";
}

void CapDetection::reduceImageDensity() {
  inputImage.copyTo(outputImage);
  // convert to single channel -- gray
  cvtColor(outputImage, outputImage, CV_BGR2GRAY);
  Mat canny;
  outputImage =
      ::reduceImageDensity(outputImage, capNoFilterVariables.minThresholdValue, capNoFilterVariables.filterKernelSize);
}

void CapDetection::applyHoughCircleTransform() {
  reduceImageDensity();
  getCapsUsingBlobs();
  if (false) {
    getCapsUsingHough();
  }
  // save blobs results
  if (false) {
    ::saveImage(masterproject::cwd + "/meeting-13/results/result.bmp",
                inputImage);
  }
}
void CapDetection::getCapsUsingBlobs() {
  SimpleBlobDetector::Params params;

  params.filterByArea = false;
  params.filterByCircularity = false;
  // high convexity i.e. no breakage in the shape
  // near to the circle
  params.filterByConvexity = true;
  params.minConvexity = capNoFilterVariables.minConvex;
  params.maxConvexity = 1.0;
  // high intertia i.e. blob should not be
  // elongated but it should be near circle shape
  params.filterByInertia = true;
  params.minInertiaRatio = 0.6;
  params.maxInertiaRatio = 1;
  // filter blob based on black colors
  // threshold is applied in a way
  // that it makes caps as black (0,0,0)
  params.filterByColor = true;
  params.blobColor = 0;

  // Set up the detector with default parameters.
  SimpleBlobDetector detector(params);
  vector<KeyPoint> keypoints;
  detector.detect(outputImage, keypoints);
  // usually bottle caps are within
  // below range. providing boundary condition
  // reduces noise.
  const float minArea = 30.0;
  const float maxArea = 100.0;

  vector<KeyPoint> unqiue_keypoints;
  for (const auto& point : keypoints) {
    if (point.size > minArea && point.size < maxArea) {
      // save caps points
      unqiue_keypoints.push_back(point);
      // debug caps points
      cout << "size: " << point.size 
           << "   x: " << point.pt.x
           << "   y: " << point.pt.y << endl;
      // draw caps points -- cross
      drawMarker(inputImage,                         // image on which to draw
                 cv::Point(point.pt.x, point.pt.y),  // coordinates
                 cv::Scalar(255, 255, 255),          // color -- red
                 MARKER_CROSS,                       // cross sign
                 10,                                 //
                 capNoFilterVariables.markerSize);   // size of the cross
    }
  }
  // draw caps points -- circle
  drawKeypoints(inputImage,             // input image
                unqiue_keypoints,       // keypoints found using blob detection
                inputImage,             // output image
                Scalar(255, 255, 255),  // colour for the points
                DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
}
void CapDetection::getCapsUsingHough() {
  // hough circle to determine bottle caps
  vector<Vec3f> bottleCaps;
  // hough circle gives us [0]->x, [1]->y, [2]->radius
  HoughCircles(outputImage, bottleCaps, CV_HOUGH_GRADIENT, 1,
               outputImage.rows / 2,
               // canny parameters
               20, 10,
               // min_radius & max_radius
               minRadius, maxRadius);
  // draw the caps
  for (size_t i = 0; i < bottleCaps.size(); i++) {  // just two caps at a time
    Vec3i cap = bottleCaps[i];
    circle(inputImage,             // image
           Point(cap[0], cap[1]),  // x, y of circle (to be drawn)
           cap[2],                 // radius of the circle (to be drawn)
           Scalar(0, 0, 255),      // red color
           3,                      // thickness of the point
           8, 0);
    // draw the center of bottle cap
    circle(inputImage, Point(cap[0], cap[1]), 1, Scalar(255, 255, 255), 3, 8,
           0);
  }
}
void CapDetection::getBottlesCircles() {
  // hough circle to determine bottle radius
  vector<Vec3f> bottleRadius;
  // find the bottle
  HoughCircles(outputImage, bottleRadius, CV_HOUGH_GRADIENT, 1,
               // change this value to detect circles with different distances
               // to each other
               outputImage.rows / 4,
               // canny parameters
               200, 10,
               // min_radius & max_radius
               70, 100);
  // draw the bottles
  for (size_t i = 0; i < bottleRadius.size(); i++) {
    Vec3i bottle = bottleRadius[i];
    // draw the circle for bottle
    circle(inputImage,                   // image
           Point(bottle[0], bottle[1]),  // x, y of circle (to be drawn)
           bottle[2],                    // radius of the circle (to be drawn)
           Scalar(255, 255, 255),        // red color
           3,                            // thickness of the point
           8, 0);
    // draw the center of bottle cap
    circle(inputImage, Point(bottle[0], bottle[1]), 1, Scalar(255, 255, 255), 3,
           8, 0);
  }
}
