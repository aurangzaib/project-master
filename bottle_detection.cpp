#include <typeinfo>
#include "header.h"
#include "stdafx.h"
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

Mat reduceImageDensity(
    Mat reduceDensityImage,             // image for operation
    const unsigned minThreshValue,      // minimum threshold value
    const unsigned method,              // binary, inverse etc
    const unsigned filterKernelSize) {  // kernel size
  Mat median;
  medianBlur(reduceDensityImage,  // source
             median,              // destination
             filterKernelSize     // aperture size (odd and >1)
             );
  threshold(median, reduceDensityImage, minThreshValue, 255, method);
  return reduceDensityImage;
}

struct region {
  const int x = 30;
  const int y = 30;
  const int width = 200;
  const int height = 350;
} roi;

struct BottleNoFilter {
  const float averageBlobArea = 11.5;
  const int minThresholdValue = 150;
  const int filterKernelSize = 1;
  const int markerSize = 5;
} bottleNoFilterVariable;

struct BottleWithFilter {
  const float averageBlobArea = 2.0;
  const int minThresholdValue = 30;
  const int filterKernelSize = 1;
  const int markerSize = 2;
} bottleWithFilterVariable;

auto bottle_flag = bottleNoFilterVariable;

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
  if (false) {
    saveImage(imagePath, canny);
  }
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

  vector<int> lineCoordinates;

  for (int loopVar = 0; loopVar < lines.size(); loopVar++) {
    // hough coordinates [rho, theta]
    float rho = lines[loopVar][0];    // first row
    float theta = lines[loopVar][1];  // second row
    Point pt1, pt2;                   // cartesian coordinates [x, y]

    double a = cos(theta), b = sin(theta);

    double x0 = rho * a;  // r * cos(theta)
    double y0 = rho * b;  // r * sin(theta)

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
  saveImage(masterproject::cwd + "/image-1.bmp", I1);
  saveImage(masterproject::cwd + "/image-2.bmp", I2);

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
  detectionImage =
      reduceImageDensity(detectionImage,                 // image
                         bottle_flag.minThresholdValue,  // threshold
                         CV_THRESH_BINARY,
                         bottle_flag.filterKernelSize  // filter size
                         );
  if (SHOW_IMAGE) imshow ("laying threshold", detectionImage);
  SimpleBlobDetector::Params params;
  params.filterByArea = false;
  params.filterByInertia = false;
  params.filterByCircularity = false;
  params.filterByConvexity = false;
  params.filterByColor = true;
  params.blobColor = 255;

  // Set up the detector with default parameters.
  SimpleBlobDetector detector(params);
  vector<KeyPoint> keypoints;
  detector.detect(detectionImage, keypoints);

  // TODO: improve perforamce,
  // dont need to save in unique_keypoints
  // show the results instead of saving in array
  vector<KeyPoint> unqiue_keypoints;
  auto imageWidth = detectionImage.size().width;
  for (const auto& point : keypoints) {
    if (point.size > bottle_flag.averageBlobArea) {
      // point should not be on the edges of the images
      // this check reduces noise points
      if (point.pt.x > 10 && abs(point.pt.x - imageWidth) > 10) {
        unqiue_keypoints.push_back(point);
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

  for (const auto& p : unqiue_keypoints) {
      drawMarker(inputImage, 
                   Point(p.pt.x, p.pt.y),
                   Scalar(0, 0, 255),
                   MARKER_CROSS, 10, 
                   bottle_flag.markerSize);
  }
  // save blobs results
  if (false) {
    ::saveImage(masterproject::cwd + "/meeting-14/results-filter/result.bmp",
                inputImage);
  }
}

void BottleDetection::getDarkBottles() {
  unsigned minThresholdValue = 80;
  unsigned filterKernelSize = 1;
  Mat detectionImage;
  inputImage.copyTo(detectionImage);
  detectionImage = reduceImageDensity(detectionImage,     // image
                                      minThresholdValue,  // threshold
                                      CV_THRESH_BINARY_INV,
                                      filterKernelSize  // filter size
                                      );
  if (SHOW_IMAGE) imshow ("dark threshold", detectionImage);
  // Create a structuring element (SE)
  int morph_size = 2;
  Mat element = getStructuringElement(
      MORPH_RECT, Size(2 * morph_size + 1, 4 * morph_size + 1),
      Point(morph_size, morph_size));

  // Apply the specified morphology operation

  for (unsigned loop = 0; loop < 10; loop++) {
    morphologyEx(detectionImage,  // source
                 detectionImage,  // destination
                 MORPH_OPEN,      // morphology operation
                 element,         // structuring element
                 Point(-1, -1),   // center
                 5);              //
  }
   if (SHOW_IMAGE) imshow("opening", detectionImage);
  ::saveImage(masterproject::cwd + "/meeting-12/lower/morphological_opening/result.bmp", detectionImage);
  SimpleBlobDetector::Params params;
  params.filterByArea = false;
  params.filterByCircularity = false;
  params.filterByConvexity = false;
  params.filterByInertia = false;
  params.filterByColor = true;
  params.blobColor = 255;
  SimpleBlobDetector detector(params);

  vector<KeyPoint> keypoints;
  detector.detect(detectionImage, keypoints);

  vector<KeyPoint> unqiue_keypoints;
  auto minArea = 80, maxArea = 200;

  for (const auto& p : keypoints) {
    auto size = p.size;
    auto imageSize = detectionImage.size();
    // blob area condition
    if (size >= minArea && size <= maxArea) {
      // points should not be on the edges of the images
      // this is to make sure to ignore noise results
      if (p.pt.x < imageSize.width - 20 && p.pt.x > 20) {
        unqiue_keypoints.push_back(p);
        cv::drawMarker(inputImage,             // image
                       Point(p.pt.x, p.pt.y),  // coordinates
                       Scalar(255, 0, 0),      // color
                       MARKER_CROSS,
                       10,
                       bottle_flag.markerSize);   // options
      }
    }
  }
}

void BottleDetection::computeResults() {
  Mat thresh = applyFilters(inputImage);
  applyProbabilisticHoughTransform(thresh);
}
