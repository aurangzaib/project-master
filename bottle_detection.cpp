#include "stdafx.h"
#include <typeinfo>
#include "header.h"
#include <string>
// region of interest
// coordinates and size
struct region {
  const int x = 30;
  const int y = 30;
  const int width = 200;
  const int height = 350;
} roi;
// common parameters when
// no filter is used
struct BottleNoFilter {
  const float averageBlobArea = 11.5;
  const unsigned minThresholdValue = 130;
  const unsigned filterKernelSize = 5;
  const unsigned markerSize = 15;
  const unsigned markerThickness = 3;
  const unsigned boundaryThreshold = 40;
} bottleNoFilterVariable;
// common parameters when
// (analyzer and polarizer) filter is used
struct BottleWithFilter {
  const float averageBlobArea = 2.0;
  const unsigned minThresholdValue = 15;
  const unsigned filterKernelSize = 1;
  const unsigned markerSize = 8;
  const unsigned markerThickness = 2;
  const unsigned boundaryThreshold = 40;
} bottleWithFilterVariable;

// common parameters when
// (analyzer and polarizer) filter is used
struct DarkBottleFilter {
  const float minBlobArea = 80;
  const unsigned maxBlobArea = 200;
  const unsigned minThresholdValue = 60;
  const unsigned filterKernelSize = 1;
  const unsigned markerSize = 15;
  const unsigned markerThickness = 3;
  const unsigned boundaryThreshold = 40;
  const unsigned morphSize = 5;
} darkBottleVariable;

// placeholder for the parameter
auto bottle_flag = bottleNoFilterVariable;
auto dark_flag = darkBottleVariable;

// helper function to save
// image with appending the path
void saveImage(string imagePath, const Mat image) {
  time_t timev;
  time(&timev);
  size_t position = imagePath.find('.');
  string toReplace = "-" + to_string(time(&timev));
  imagePath.replace(position, 0, toReplace);
  imwrite(imagePath, image);
}

Mat reduceDensity(const Mat algorithmImage,       // image for operation
                  const unsigned minThreshValue,  // minimum threshold value
                  const unsigned method,          // binary, inverse etc
                  const unsigned filterKernelSize,
                  const string caller) {  // kernel size
  Mat resultImage;
  algorithmImage.copyTo(resultImage);
  Mat median;
  medianBlur(resultImage,      // source
             median,           // destination
             filterKernelSize  // aperture size (odd and >1)
             );
  if (SAVE_RESULTS)
    saveImage(prj::cwd + "/results/median-" + caller + ".bmp", median);
  threshold(median, resultImage, minThreshValue, 255, method);
  if (SAVE_RESULTS)
    saveImage(prj::cwd + "/results/threshold-" + caller + ".bmp", resultImage);
  return resultImage;
}

class BottleDetection {
 private:
  string imagePath;
  vector<int> linePoints;
  vector<int> lineUniquePoints;

 public:
  Mat inputImage, outputImage;
  BottleDetection();
  BottleDetection(const string);
  BottleDetection(Mat&);
  const Mat applyFilters(const Mat);
  void findLineUniquePoints();
  void computeResults();
  void applyHoughTransform(const Mat);
  void applyProbabilisticHoughTransform(const Mat);
  void getPSNR(Mat&, Mat&);
  void getBottles();
  void getDarkBottles();
  void getRegionOfInterest(Mat&, const int, const int, const int, const int);
};

// default ctor
BottleDetection::BottleDetection() { imagePath = "/Meeting-7/original-1.bmp"; }

// ctor with image path as string
BottleDetection::BottleDetection(const string imagePath)
    : imagePath(imagePath) {
  inputImage = imread(imagePath);
}

// ctor with image as matrix
BottleDetection::BottleDetection(Mat& inputImage) : inputImage(inputImage) {}
// find the hough line
const Mat BottleDetection::applyFilters(const Mat image) {
  // grayscale conversion
  Mat gray;
  cvtColor(inputImage,     // source
           gray,           // destination
           COLOR_BGR2GRAY  // src, output, option
           );
  // median blur to reduce the noise
  Mat median;
  medianBlur(gray,    // source
             median,  // destination
             7);      // aperture size (odd and >1)
  // canny contour detection
  Mat canny;
  Canny(median,  // source
        canny,   // destination
        120,     // low threshold
        200,     // high threshold
        3        // kernel size 3x3
        );
  // thresholding
  Mat thresh;
  threshold(median, thresh, 170, 255, THRESH_BINARY);
  return thresh;
}
// draw a line using results of hough line transform
void BottleDetection::applyHoughTransform(const Mat thresh) {
  // to save lines from hough transform
  // Vector<Vec4i> is used for probabilistic hough transform
  vector<Vec2f> lines;
  // save a copy of inputImage in outputImage
  inputImage.copyTo(outputImage);
  // it will generate the accumulator cell
  // which is a 2xn matrix containing r->firstrow and theta->secondrow
  HoughLines(thresh,               // source
             lines,                // destination
             1,                    // rho resolution (0...2pi)
             180 * (CV_PI / 180),  // theta resolution (1 degree here)
             10,  // threshold -- vmin # of intersection to detect a line
             0,   // min. # of points to form a line
             0    // max gap b/w 2 points to be consider as 1 line.
             );
  // vector for line coordinates
  vector<int> lineCoordinates;
  for (unsigned loopVar = 0; loopVar < lines.size(); loopVar++) {
    // hough coordinates [rho, theta]
    float rho = lines[loopVar][0];    // first row
    float theta = lines[loopVar][1];  // second row
    Point pt1, pt2;                   // cartesian coordinates [x, y]

    double a = cos(theta), b = sin(theta);
    // r * cos(theta)
    double x0 = rho * a;
    // r * sin(theta)
    double y0 = rho * b;

    // rows are used to draw line to cover the whole vertical space for
    // particular x
    // extrapolation to draw a line

    // point 1
    pt1.x = cvRound(x0 + inputImage.rows * (-b));
    pt1.y = cvRound(y0 + inputImage.rows * (a));
    // point 2
    pt2.x = cvRound(x0 - inputImage.rows * (-b));
    pt2.y = cvRound(y0 - inputImage.rows * (a));

    // save x coordinates
    lineCoordinates.push_back(x0);  // push point1 x coords

    line(outputImage, pt1, pt2, Scalar(0, 0, 255),  // red colour
         1,                                         // line width
         CV_AA);
  }

  sort(lineCoordinates.begin(), lineCoordinates.end(),
       greater<int>());  // sort in descending order

  linePoints = lineCoordinates;
}

// draw a line using results of hough line transform
void BottleDetection::applyProbabilisticHoughTransform(const Mat thresh) {
  // to save lines from hough transform
  // Vector<Vec4i> is used for probabilistic hough transform
  vector<Vec4i> lines;

  // it will generate the accumulator cell
  // which is a 2xn matrix containing r->firstrow and theta->secondrow
  HoughLinesP(thresh,  // source
              lines,  // destination (each line: x-start, y-start, x-end, y-end)
              1,      // rho resolution (0...2pi)
              180 * (CV_PI / 180),  // theta resolution (1 degree here)
              10,  // threshold -- vmin # of intersection to detect a line
              10,  // min. # of points to form a line
              10   // max gap b/w 2 points to be consider as 1 line.
              );

  getRegionOfInterest(inputImage, roi.x, roi.y, roi.width, roi.height);
  // save a copy of inputImage in outputImage
  inputImage.copyTo(outputImage);
  vector<int> lineCoordinates;

  for (size_t i = 0; i < lines.size(); i++) {
    Vec4i singleLine = lines[i];

    line(outputImage,                          // destination
         Point(singleLine[0], singleLine[1]),  // x and y of start point
         Point(singleLine[2], singleLine[3]),  // x and y of end point
         Scalar(0, 0, 255),                    // color of line -- red
         1,                                    // thickness of line -- thin
         CV_AA                                 //
         );

    lineCoordinates.push_back(singleLine[0]);
    lineCoordinates.push_back(singleLine[3]);
  }

  // sort in ascending order all the x axis values
  sort(lineCoordinates.begin(),  // first element
       lineCoordinates.end(),    // last element
       greater<int>());          // sort in descending order

  // removeDuplicate(lineCoordinates);
  linePoints = lineCoordinates;
}

void BottleDetection::findLineUniquePoints() {
  vector<int> condensedArray;
  if (linePoints.size() < 3) {
    condensedArray = linePoints;
  } else {
    size_t valueRange = linePoints.at(0) - linePoints.at(linePoints.size() - 1);
    condensedArray.push_back(linePoints.at(0));
    for (int loopVar = 0; loopVar < linePoints.size() - 1; loopVar++) {
      if ((linePoints.at(loopVar) - linePoints.at(loopVar + 1)) >
          (valueRange / 2))
        condensedArray.push_back(linePoints.at(loopVar + 1));
    }
  }
  lineUniquePoints = condensedArray;
}

void BottleDetection::getPSNR(Mat& I1, Mat& I2) {
  getRegionOfInterest(I1, roi.x, roi.y, roi.width, roi.height);
  getRegionOfInterest(I2, roi.x, roi.y + 10, roi.width, roi.height);

  threshold(I1, I1, 150, 255, THRESH_BINARY);
  threshold(I2, I2, 150, 255, THRESH_BINARY);

  Mat s1;
  absdiff(I1, I2, s1);       // |I1 - I2|
  s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
  s1 = s1.mul(s1);           // |I1 - I2|^2
  Scalar s = sum(s1);        // sum elements per channel
  double sse = s.val[0] + s.val[1] + s.val[2];  // sum channels

  // for small values return zero
  if (sse <= 1e-10) {
    cout << "sse: " << sse << endl;
  } else {
    double mse = sse / (double)(I1.channels() * I1.total());
    double psnr = 10.0 * log10((255 * 255) / mse);
    cout << "SNR: " << psnr << endl;
  }
}

void BottleDetection::getRegionOfInterest(Mat& referenceImage, const int x,
                                          const int y, const int width,
                                          const int height) {
  // rectangular mask
  Rect Rec(x,      // x coordinate
           y,      // y coordinate
           width,  // width
           height  // height
           );
  // region of interest
  referenceImage = referenceImage(Rec);
}
// get the blob of white colors
// in our case, these are the areas from
// where the light is coming out of chain
// we get the blobs align with their pose and size
// when the size is more than a threshold value ...
// ... (say average of the blobs sizes) it means
// something is present their besides only the chain
void BottleDetection::getBottles() {
  Mat detectionImage;
  inputImage.copyTo(detectionImage);
  // reduce image density using
  // thresholding and guassian filter
  detectionImage = reduceDensity(detectionImage,                 // image
                                 bottle_flag.minThresholdValue,  // threshold
                                 CV_THRESH_BINARY,
                                 bottle_flag.filterKernelSize,  // filter size
                                 "bottles");
  // debug -- image
  if (SHOW_IMAGE) imshow("laying threshold", detectionImage);
  // set the criteria for the blobs
  SimpleBlobDetector::Params params;
  params.filterByArea = false;
  params.filterByInertia = false;
  params.filterByCircularity = false;
  params.filterByConvexity = false;
  params.filterByColor = true;
  params.blobColor = 255;
  // Set up the detector with default parameters.
  Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
  vector<KeyPoint> keypoints;
  detector->detect(detectionImage, keypoints);
  // TODO: improve perforamce,
  // dont need to save in unique_keypoints
  // show the results instead of saving in array
  vector<KeyPoint> unqiue_keypoints;
  auto imageSize = detectionImage.size();
  // iterate keypoints and draw points which
  // match the conditions
  Mat debugImage;
  detectionImage.copyTo(debugImage);
  for (const auto& p : keypoints) {
    if (p.size > bottle_flag.averageBlobArea) {
      // points should not be on the edges of the images
      // this is to make sure to ignore noise results
      const bool xBoundary = p.pt.x < imageSize.width - 40 && p.pt.x > 40;
      const bool yBoundary = p.pt.y < imageSize.height - 40 && p.pt.y > 40;
      if (xBoundary && yBoundary) {
        // draw the marker in the found regions
        drawMarker(inputImage, Point(p.pt.x, p.pt.y),  // coordinates
                   Scalar(0, 0, 255),                  // red
                   MARKER_CROSS,                       // marker type
                   bottle_flag.markerSize,             // size of marker
                   bottle_flag.markerThickness);
        // --- ONLY DEBUG PURPOSE ---
        drawMarker(debugImage, Point(p.pt.x, p.pt.y),  // coordinates
                   Scalar(0, 0, 255),                  // red
                   MARKER_CROSS,                       // marker type
                   bottle_flag.markerSize,             // size of marker
                   bottle_flag.markerThickness);
      }
    }
  }

  if (false) {
    float totalArea = 0;
    // find the average area from the teach image
    for (const auto& point : keypoints) {
      totalArea += point.size;
    }
    totalArea /= keypoints.size();
  }
  // save blobs results
  if (SAVE_RESULTS) saveImage(prj::cwd + "/results/bottle.bmp", debugImage);
}

void BottleDetection::getDarkBottles() {
  Mat detectionImage;
  inputImage.copyTo(detectionImage);
  // reduce image density using
  // thresholding and guassian filter
  detectionImage = reduceDensity(detectionImage,               // image
                                 dark_flag.minThresholdValue,  // threshold
                                 CV_THRESH_BINARY_INV,
                                 dark_flag.filterKernelSize,  // filter size
                                 "dark-bottles");
  if (SHOW_IMAGE) imshow("dark threshold", detectionImage);
  // Create a structuring element (SE)
  Mat element = getStructuringElement(
      MORPH_RECT,
      Size(1 * dark_flag.morphSize + 1, 1 * dark_flag.morphSize + 1),
      Point(dark_flag.morphSize, dark_flag.morphSize));
  // Apply the specified morphology operation
  for (unsigned loop = 0; loop < 5; loop++) {
    morphologyEx(detectionImage,  // source
                 detectionImage,  // destination
                 MORPH_OPEN,      // morphology operation
                 element,         // structuring element
                 Point(-1, -1),   // center
                 5);              //
  }
  // debug -- image
  if (SHOW_IMAGE) imshow("opening", detectionImage);
  // save blobs results
  if (SAVE_RESULTS)
    saveImage(prj::cwd + "/results/opening.bmp", detectionImage);
  // set the criteria for the blobs
  SimpleBlobDetector::Params params;
  params.filterByArea = false;
  params.filterByCircularity = false;
  params.filterByConvexity = false;
  params.filterByInertia = false;
  params.filterByColor = true;
  params.blobColor = 255;
  Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
  // vector of keypoints
  vector<KeyPoint> keypoints;
  // apply blob detection with the given criteria
  detector->detect(detectionImage, keypoints);
  // iterate keypoints and draw points which
  // match the conditions
  Mat debugImage;
  /** DEBUG PURPOSE **/
  detectionImage.copyTo(debugImage);
  auto imageSize = detectionImage.size();
  for (const auto& p : keypoints) {
    auto size = p.size;
    // blob area condition
    bool lowerBoundary = size >= dark_flag.minBlobArea;
    bool upperBoundary = size <= dark_flag.maxBlobArea;
    if (lowerBoundary && upperBoundary) {
      // points should not be on the edges of the images
      // this is to make sure to ignore noise results
      const bool xBoundary =
          p.pt.x < imageSize.width - bottle_flag.boundaryThreshold &&
          p.pt.x > dark_flag.boundaryThreshold;
      const bool yBoundary =
          p.pt.y < imageSize.height - bottle_flag.boundaryThreshold &&
          p.pt.y > dark_flag.boundaryThreshold;
      if (xBoundary && yBoundary) {
        // draw the marker in the found regions
        cv::drawMarker(inputImage,             // image
                       Point(p.pt.x, p.pt.y),  // coordinates
                       Scalar(255, 0, 0),      // color
                       MARKER_CROSS,           // marker type
                       dark_flag.markerSize,   // size of marker
                       dark_flag.markerThickness);
        if (ENABLE_DEBUGGER)
          cv::drawMarker(debugImage,             // image
                         Point(p.pt.x, p.pt.y),  // coordinates
                         Scalar(255, 0, 0),      // color
                         MARKER_CROSS,           // marker type
                         dark_flag.markerSize,   // size of marker
                         dark_flag.markerThickness);
      }
    }
  }
  if (SAVE_RESULTS && ENABLE_DEBUGGER)
    saveImage(prj::cwd + "/results/dark.bmp", debugImage);
}

void BottleDetection::computeResults() {
  Mat thresh = applyFilters(inputImage);
  applyProbabilisticHoughTransform(thresh);
}
