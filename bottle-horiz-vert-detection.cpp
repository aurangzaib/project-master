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

// helper function to save
// image with appending the path
void saveImage(const string imagePath, const Mat image) {
  time_t timev;
  time(&timev);
  size_t position = imagePath.find('.');
  cout << "position: " << position << endl;
  string resultPath = imagePath;
  string toReplace = "-" + to_string(time(&timev));
  resultPath.replace(position, 0, toReplace);
  imwrite(resultPath, image);
}

struct region {
  static const int x = 100;
  static const int y = 70;
  static const int width = 200;
  static const int height = 350;
} roi;

Mat getRegionOfInterest(const Mat referenceImage, const int x, const int y,
                        const int width, const int height) {
  // rectangular mask
  Rect Rec(x,      // x coordinate
           y,      // y coordinate
           width,  // width
           height  // height
           );
  // region of interest
  return referenceImage(Rec);
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
  const Mat applyFilters(const Mat);
  void findLineUniquePoints();
  void computeResults();
  void applyHoughTransform(const Mat);
  void applyProbabilisticHoughTransform(const Mat);
  void getPSNR(const Mat&, const Mat&);
};

// default ctor
BottleDetection::BottleDetection() { imagePath = "/Meeting-7/original-1.bmp"; }

// ctor with image path as string
BottleDetection::BottleDetection(const string imagePath)
    : imagePath(imagePath) {
  cout << "image path: " << imagePath << endl;
  inputImage = imread(imagePath);
}

// ctor with image as matrix
BottleDetection::BottleDetection(const Mat inputImage)
    : inputImage(inputImage) {}

// find the hough line
const Mat BottleDetection::applyFilters(const Mat image) {
  // grayscale conversion
  Mat gray;
  cvtColor(inputImage,     // source
           gray,           // destination
           COLOR_BGR2GRAY  // src, output, option
           );
  imshow("gray: ", gray);

  // median blur to reduce the noise
  Mat median;
  medianBlur(gray,    // source
             median,  // destination
             7        // aperture size (odd and >1)
             );
  imshow("median: ", median);

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
  imshow("thresh", thresh);
  if (false /*!imagePath.empty()*/) {
    saveImage(imagePath, canny);
  }

  // Mat regionOfInterest =
  //     getRegionOfInterest(thresh, roi.x, roi.y, roi.width, roi.height);
  // imshow("region of interest", regionOfInterest);

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

    //    cout << "x: " << x0 << endl;

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

  Mat inputImageMask =
      getRegionOfInterest(inputImage, roi.x, roi.y, roi.width, roi.height);
  // save a copy of inputImage in outputImage
  inputImageMask.copyTo(outputImage);
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
    cout << "range is: " << valueRange << endl;
    condensedArray.push_back(linePoints.at(0));
    for (int loopVar = 0; loopVar < linePoints.size() - 1; loopVar++) {
      //      cout << endl
      //           << "comparing " << linePoints.at(loopVar) << " with "
      //           << linePoints.at(loopVar + 1) << endl;
      if ((linePoints.at(loopVar) - linePoints.at(loopVar + 1)) >
          (valueRange / 2))
        condensedArray.push_back(linePoints.at(loopVar + 1));
    }
  }
  lineUniquePoints = condensedArray;
}

void BottleDetection::computeResults() {
  Mat thresh = applyFilters(inputImage);
  applyProbabilisticHoughTransform(thresh);

  // find unique points
  findLineUniquePoints();
  // cout << endl << endl << "values selected are: " << endl;
  // for (const auto &point : linePoints) cout << point << endl;

  if (lineUniquePoints.size() > 1) {
    int width = abs(lineUniquePoints.front() - lineUniquePoints.at(1));
    //    cout << endl << endl << "width is found: " << width << endl;

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
  waitKey();
}

void BottleDetection::getPSNR(const Mat& I1, const Mat& I2) {
  Mat im1 = getRegionOfInterest(I1, roi.x, roi.y, roi.width, roi.height);
  Mat im2 = getRegionOfInterest(I2, roi.x, roi.y + 10, roi.width, roi.height);
  threshold(im1, im1, 150, 255, THRESH_BINARY);
  threshold(im2, im2, 150, 255, THRESH_BINARY);
  saveImage(masterproject::prjdir + "/image-1.bmp", im1);
  saveImage(masterproject::prjdir + "/image-2.bmp", im2);
  imshow("1st", im1);
  imshow("2nd", im2);
  Mat s1;
  absdiff(im1, im2, s1);     // |I1 - I2|
  s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
  s1 = s1.mul(s1);           // |I1 - I2|^2
  Scalar s = sum(s1);        // sum elements per channel
  double sse = s.val[0] + s.val[1] + s.val[2];  // sum channels

  // for small values return zero
  if (sse <= 1e-10) {
    cout << "sse: " << sse << endl;
  } else {
    double mse = sse / (double)(im1.channels() * im1.total());
    double psnr = 10.0 * log10((255 * 255) / mse);
    cout << "SNR: " << psnr << endl;
  }
  waitKey(0);
}
