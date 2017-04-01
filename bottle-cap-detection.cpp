#include "bottle-horiz-vert-detection.cpp"
#include "header.h"
#include "stdafx.h"
class CapDetection {
 private:
  string imagePath;
  unsigned minRadius, maxRadius;
  Mat inputImage, outputImage;

 public:
  CapDetection();
  CapDetection(const string);
  CapDetection(const Mat, unsigned, unsigned);
  void reduceImageDensity();
  void applyHoughCircleTransform();
  void getCapsCircles();
  void getBottlesCircles();
};

CapDetection::CapDetection() {
  string imagePath = masterproject::cwd + "meeting-5/cap-teach-5.bmp";
  inputImage = imread(imagePath);
  minRadius = 30;
  maxRadius = 40;
}

CapDetection::CapDetection(const string imagePath) : imagePath(imagePath) {
  inputImage = imread(imagePath);
  minRadius = 30;
  maxRadius = 40;
}

CapDetection::CapDetection(const Mat inputImage, unsigned minRadius,
                           unsigned maxRadius)
    : inputImage(inputImage), minRadius(minRadius), maxRadius(maxRadius) {
  imagePath = "";
}

void CapDetection::reduceImageDensity() {
  BottleDetection detectInstance;
  inputImage.copyTo(outputImage);
  // convert to single channel -- gray 
  cvtColor(outputImage, outputImage, CV_BGR2GRAY);
  detectInstance.reduceImageDensity(inputImage, false,
                                    0,  // min threshold value
                                    0   // filter kernel size
                                    );
  detectInstance.reduceImageDensity(outputImage, true,
                                    28,  // min threshold value
                                    15   // filter kernel size
                                    );
}

void CapDetection::applyHoughCircleTransform() {
  reduceImageDensity();
  getCapsCircles();
}

void CapDetection::getCapsCircles() {
  // hough circle to determine bottle caps
  vector<Vec3f> bottleCaps;
  imshow("image for hough", outputImage);
  // hough circle gives us [0]->x, [1]->y, [2]->radius
  HoughCircles(outputImage, bottleCaps, CV_HOUGH_GRADIENT, 1,
               outputImage.rows / 6,
               // canny parameters
               200, 10,
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
  imshow("caps", inputImage);
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
