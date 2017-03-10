#include "stdafx.h"
#include "header.h"

class BottleDetection {
 private:
  string imagePath;
  Mat inputImage, outputImage;
  vector<int> linePoints;
  vector<int> lineUniquePoints;

 public:
  BottleDetection();
  BottleDetection(const string);
  void findLinePoints(const Mat image, const bool useImage);
  void findLineUniquePoints();
  void computeResults(const Mat image, const bool useImage);
};

// default ctor
BottleDetection::BottleDetection() { imagePath = "/Meeting-7/original-1.bmp"; }
// ctor
BottleDetection::BottleDetection(const string imagePath)
    : imagePath(imagePath) {}
// find the hough line
void BottleDetection::findLinePoints(const Mat image, const bool useImage) {
  // input image
  // if no image provide then fetch the image
  inputImage =
      (useImage == true) 
      ? image 
      : imread(masterproject::prjdir + imagePath, CV_LOAD_IMAGE_COLOR);
  
    // grayscale conversion
  Mat gray;
  cvtColor(inputImage, gray, COLOR_BGR2GRAY);  // src, output, option
  // median blur to reduce the noise
  Mat median;
  medianBlur(gray,    // source
             median,  // destination
             9        // aperture size (odd and >1)
             );
  // canny contour detection
  Mat canny;
  Canny(median,  // source
        canny,   // destination
        120,     // low threshold
        200,     // high threshold
        3        // kernel size 3x3
        );
  // thesholding
  Mat thresh;
  threshold(gray, thresh, 100, 255, THRESH_BINARY);
  // to save lines from hough transform
  vector<Vec2f> lines;
  // it will generate the accumulator cell
  // which is a 2xn matric contaning r->firstrow and theta->secondrow
  HoughLines(thresh,             // source
             lines,              // destination
             1,                  // rho resolution
             180 * CV_PI / 180,  // theta resolution (1 degree here)
             10,                 // min # of intersection to detect a line
             0, 0                // srn and stn
             );
  // save a copy of inputImage in outputImage
  inputImage.copyTo(outputImage);

  vector<int> lineCoordinates;

  for (int loopVar = 0; loopVar < lines.size(); loopVar++) {
    // hough coordinates [row, theta]
    float rho = lines[loopVar][0];    // first row
    float theta = lines[loopVar][1];  // second row
    Point pt1, pt2;                   // cartesian coordinates [x, y]

    double a = cos(theta), b = sin(theta);

    double x0 = rho * a;  // r * cos(theta)
    double y0 = rho * b;  // r * sin(thera)

    // point 1
    pt1.x = cvRound(x0 + 1000 * (-b));
    pt1.y = cvRound(y0 + 1000 * (a));

    // point 2
    pt2.x = cvRound(x0 - 1000 * (-b));
    pt2.y = cvRound(y0 - 1000 * (a));

    lineCoordinates.push_back(pt1.x);  // push point1 coords
    line(outputImage, pt1, pt2, Scalar(0, 0, 255), 1,
         CV_AA);  // draw line using point1 and point2. red color
  }

  sort(lineCoordinates.begin(), lineCoordinates.end(),
       greater<int>());  // sort in descending order
  linePoints = lineCoordinates;
    
    imshow("1-original image: ", inputImage);
    imshow("2-gray image", gray);
    imshow("3-median blur image", median);
    imshow("4-canny contour image", canny);
    imshow("5-threshold image", thresh);
}

void BottleDetection::findLineUniquePoints() {
  vector<int> condensedArray;

  for (int loopVar = 0; loopVar < linePoints.size(); loopVar++) {
    cout << "value: " << linePoints.at(loopVar) << endl;
  }

  if (linePoints.size() < 3) {
    condensedArray = linePoints;
  }

  else {
    size_t valueRange = linePoints.at(0) - linePoints.at(linePoints.size() - 1);
    cout << endl;
    condensedArray.push_back(linePoints.at(0));
    for (int loopVar = 0; loopVar < linePoints.size() - 1; loopVar++) {
      cout << endl
           << "comparing " << linePoints.at(loopVar) << " with "
           << linePoints.at(loopVar + 1);
      if ((linePoints.at(loopVar) - linePoints.at(loopVar + 1)) >
          valueRange / 2)
        condensedArray.push_back(linePoints.at(loopVar + 1));
    }
  }
  lineUniquePoints = condensedArray;
}

void BottleDetection::computeResults(const Mat image, const bool useImage) {
  findLinePoints(image, useImage);
  findLineUniquePoints();
  cout << endl << endl << "values selected are: " << endl;
  for (const auto &point : lineUniquePoints) cout << point << " ";

  if (lineUniquePoints.size() > 1) {
    int width = abs(lineUniquePoints.front() - lineUniquePoints.at(1));
    cout << endl << endl << "width is found: " << width << endl;

    if (width < 5) {
      cout << "there is no bottle" << endl;
    } else if (width > 5 && width < 80) {
      cout << "bottle is horizontal" << endl;
    } else if (width >= 80 && width < 255) {
      cout << "bottle is vertical" << endl;
    } else {
      cout << "system could not found the situation" << endl;
    }
  }

  else {
    cout << "there is no bottle" << endl;
  }
  imshow("6-hough line transform ", outputImage);
  // waitKey();
}
