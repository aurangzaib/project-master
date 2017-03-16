#include <typeinfo>
#include "header.h"
#include "stdafx.h"

void removeNumber(vector<int> numbers, int idx) {
  int size = sizeof(numbers) / sizeof(numbers[0]);
  for (size_t loop = idx; loop < size - 1; loop++) {
    numbers.at(loop) = numbers[loop + 1];
  }
  size--;
}

void removeDuplicate(vector<int> numbers) {
  int i, j;
  int number;
  int size = sizeof(numbers) / sizeof(numbers[0]);
  for (i = 0; i < size; i++) {
    number = numbers[i];
    for (j = i + 1; j < size; j++) {
      if (number == numbers[j]) {
        removeNumber(numbers, j);
        j--;
      }
    }
  }
}

class BottleDetection {
 private:
  string imagePath;
  Mat inputImage, outputImage;
  vector<int> linePoints;
  vector<int> lineUniquePoints;

 public:
  BottleDetection();
  BottleDetection(const string);
  BottleDetection(const Mat);
  const Mat applyFilters(const Mat, const bool);
  void findLineUniquePoints();
  void computeResults(const Mat, const bool);
  void applyHoughTransform(const Mat);
  void applyProbabilisticHoughTransform(const Mat);
};

// default ctor
BottleDetection::BottleDetection() { imagePath = "/Meeting-7/original-1.bmp"; }

// ctor with image path as string
BottleDetection::BottleDetection(const string imagePath)
    : imagePath(imagePath) {}

// ctor with image as matrix
BottleDetection::BottleDetection(const Mat inputImage)
    : inputImage(inputImage) {}

// draw a line using results of hough line transform
void BottleDetection::applyHoughTransform(const Mat thresh) {
  // to save lines from hough transform
  // Vector<Vec4i> is used for probabilistic hough transform
  vector<Vec2f> lines;

  // it will generate the accumulator cell
  // which is a 2xn matrix containing r->firstrow and theta->secondrow
  HoughLines(inputImage,           // source
             lines,                // destination
             1,                    // rho resolution (0...2pi)
             180 * (CV_PI / 180),  // theta resolution (1 degree here)
             10,  // threshold -- vmin # of intersection to detect a line
             0,   // min. # of points to form a line
             0    // max gap b/w 2 points to be consider as 1 line.
             );

  // save a copy of inputImage in outputImage
  inputImage.copyTo(outputImage);

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

    cout << "x: " << x0 << endl;

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

  removeDuplicate(lineCoordinates);
  linePoints = lineCoordinates;
}

// find the hough line
const Mat BottleDetection::applyFilters(const Mat image, const bool useImage) {
  // input image
  // if no image provide then fetch the image
  inputImage =
      (useImage == true) ? image : imread(masterproject::prjdir + imagePath,
                                          CV_LOAD_IMAGE_COLOR);

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
             15       // aperture size (odd and >1)
             );

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
  threshold(gray, thresh, 100, 255, THRESH_BINARY);

  return thresh;
}

void BottleDetection::findLineUniquePoints() {
  vector<int> condensedArray;
  if (linePoints.size() < 3) {
    condensedArray = linePoints;
  }

  else {
    size_t valueRange = linePoints.at(0) - linePoints.at(linePoints.size() - 1);
    cout << "range is: " << valueRange << endl;
    condensedArray.push_back(linePoints.at(0));
    for (int loopVar = 0; loopVar < linePoints.size() - 1; loopVar++) {
      cout << endl
           << "comparing " << linePoints.at(loopVar) << " with "
           << linePoints.at(loopVar + 1);
      if ((linePoints.at(loopVar) - linePoints.at(loopVar + 1)) >
          (valueRange / 2))
        condensedArray.push_back(linePoints.at(loopVar + 1));
    }
  }
  lineUniquePoints = condensedArray;
}

void BottleDetection::computeResults(const Mat image, const bool useImage) {
  // apply filters on the input image
  Mat thresh = applyFilters(image, useImage);

  // apply hough transform on the threshold image
  applyProbabilisticHoughTransform(thresh);

  // find unique points
  // findLineUniquePoints();
  cout << endl << endl << "values selected are: " << endl;
  for (const auto &point : linePoints) cout << point << endl;

  // if (lineUniquePoints.size() > 1) {
  //   int width = abs(lineUniquePoints.front() - lineUniquePoints.at(1));
  //   cout << endl << endl << "width is found: " << width << endl;

  //   if (width < 5) {
  //     cout << "there is no bottle" << endl;
  //   } else if (width > 5 && width < 80) {
  //     cout << "bottle is horizontal" << endl;
  //   } else if (width >= 80 && width < 255) {
  //     cout << "bottle is vertical" << endl;
  //   } else {
  //     cout << "system could not found the situation" << endl;
  //   }
  // }

  // else {
  //   cout << "there is no bottle" << endl;
  // }
  imshow("6-hough line transform ", outputImage);
  // waitKey();
}
