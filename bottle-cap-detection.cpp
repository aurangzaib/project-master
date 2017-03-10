#include "header.h"
#include "stdafx.h"

void bottleCapDetection(Mat img[], const unsigned radiusMin,
                        const unsigned radiusMax) {
  // array for images
  //  Mat img[12];

  int arraySize = 1;  // sizeof(img) / sizeof(*img); // img/img[0]

  //   reference the images in array
  // not required when acquiring from different sources
  if (false) {
    for (int loopVar = 0; loopVar < arraySize; loopVar++) {
      *(img + loopVar) =
          imread(masterproject::prjdir + "/Meeting-5/cap-teach-1.bmp",
                 CV_LOAD_IMAGE_COLOR);
    }
  }

  // apply medianBlur, thresholding and houghcircle on each image
  for (int loopVar = 0; loopVar < arraySize; loopVar++) {
    // gray scale conversion
    Mat gray, thresh, threshAdapt, threshAdapt2, threshAdapt3;
    cvtColor(*(img + loopVar), gray, COLOR_BGR2GRAY);

    // median filter to reduce noise
    // reduce unwanted objects from the image
    medianBlur(
        // source
        gray,
        // destination
        gray,
        // kernel size (square)
        15);

    // threshold to make image binary
    // ! will not work in each scenario !
    threshold(gray, thresh, 22, 255, THRESH_BINARY);  // --> disable for cap.jpg
    adaptiveThreshold(thresh, threshAdapt, 255, ADAPTIVE_THRESH_GAUSSIAN_C,
                      THRESH_BINARY, 5, 3);
    adaptiveThreshold(thresh, threshAdapt2, 255, ADAPTIVE_THRESH_MEAN_C,
                      THRESH_BINARY, 3, 1);
    adaptiveThreshold(thresh, threshAdapt3, 255, ADAPTIVE_THRESH_GAUSSIAN_C,
                      THRESH_BINARY, 7, 1);
    imshow("thesho: ", thresh);
    imshow("thesho adapt guass: ", threshAdapt);
    imshow("thesho adapt mean: ", threshAdapt2);
    imshow("thesho adapt 5 thresh: ", threshAdapt3);
    int morph_size = 2;
    Mat element = getStructuringElement(
        MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1),
        Point(morph_size, morph_size));
    cout << element;

    Mat dst;  // result matrix
              // Apply the specified morphology operation
    cv::erode(threshAdapt, dst, element);
    // morphologyEx( src, dst, MORPH_TOPHAT, element ); // here iteration=1
    //    imshow("result", dst);

    // hough circle to determine bottle caps
    vector<Vec3f> bottleCaps;
    // hough circle to determine bottle radius
    vector<Vec3f> bottleRadius;

    // hough circle gives us [0]->x, [1]->y, [2]->radius

    // find the caps
    HoughCircles(threshAdapt, bottleCaps, CV_HOUGH_GRADIENT, 1,
                 // change this value to detect circles with different distances
                 // to each other
                 gray.rows / 6,
                 // canny parameters
                 200, 10,
                 // min_radius & max_radius
                 radiusMin, radiusMax);

    // find the bottle
    HoughCircles(gray, bottleRadius, CV_HOUGH_GRADIENT, 1,
                 // change this value to detect circles with different distances
                 // to each other
                 gray.rows / 4,
                 // canny parameters
                 200, 10,
                 // min_radius & max_radius
                 70, 100);

    /* radius params:
     multicolor-bottle-caps.jpeg -- 40, 45
     blob.jpg -- 30, 32
     cap.jpg -- gray.rows/4, 82, 90
     3-light-vertical.jpg -- gray.rows/8, 20, 40
     4-light-angled.bmp -- 15, 25
     transparent-glass-bottle.jpg -- gray.rows/4, 50, 60
     glass-bottle.jpg -- 70, 75
     cap-top-view.jpg -- threshold(gray, gray, 200, 255, THRESH_BINARY_INV); 30,
     50
     Wine-bottle.jpg -- threshold(gray, gray, 50, 255, THRESH_BINARY); 30, 40
     */

    // draw the caps
    size_t loopSize = bottleCaps.size();
    cout << "number of circles: " << loopSize << endl;
    for (size_t i = 0; i < bottleCaps.size(); i++) {  // just two caps at a time

      // putText(*(img + loopVar), to_string(loopVar), (int) bottleCaps[i][0],
      // (int) bottleCaps[i][1]);

      cout << "x: " << bottleCaps[i][0] << " y: " << bottleCaps[i][1]
           << " r: " << bottleCaps[i][2] << endl;
      // draw the circle for bottle cap
      if (abs(bottleCaps[i][0]) < 1000 && abs(bottleCaps[i][1]) < 1000 &&
          abs(bottleCaps[i][1]) > 50 && abs(bottleCaps[i][2]) < 40) {
        circle(*(img + loopVar),  // image
               Point(bottleCaps[i][0],
                     bottleCaps[i][1]),  // x, y of circle (to be drawn)
               bottleCaps[i][2],         // radius of the circle (to be drawn)
               Scalar(0, 0, 255),        // red color
               3,                        // thickness of the point
               8,
               0);
        // draw the center of bottle cap
        circle(*(img + loopVar), Point(bottleCaps[i][0], bottleCaps[i][1]), 1,
               Scalar(0, 0, 255), 3, 8, 0);
      }
    }

    for (size_t i = 0; i < loopSize; i++) {
      if (abs(bottleCaps[i][0] - bottleCaps[i + 2][0]) < 550) {  // x difference
        if (abs(bottleCaps[i][1] - bottleCaps[i + 2][1]) <
            150) {  // y difference
          if (abs(bottleCaps[i][2] - bottleCaps[i + 2][2]) <
              10) {  // radius difference
            cout << "no bottle is dropped" << endl;
          } else {
            cout << "bottles cap are of different sizes" << endl;
            break;
          }
        } else {
          cout << "bottles are not aligned" << endl;
          break;
        }
      } else {
        cout << "bottles are very far. some bottles might be dropped" << endl;
        break;
      }
    }
    // draw the bottle radius

    if (false) {
      for (size_t i = 0; i < bottleRadius.size(); i++) {
        Vec3i c = bottleRadius[i];
        // draw the circle for bottle cap
        circle(*(img + loopVar),       // image
               Point(c[0], c[1]),      // x, y of circle (to be drawn)
               c[2],                   // radius of the circle (to be drawn)
               Scalar(255, 255, 255),  // red color
               3,                      // thickness of the point
               8, 0);
        // draw the center of bottle cap
        circle(*(img + loopVar), Point(c[0], c[1]), 1, Scalar(255, 255, 255), 3,
               8, 0);
      }
    }
  }

  for (int loopVar = 0; loopVar < arraySize; loopVar++) {
    // save the detected images
    // imwrite(masterproject::prjdir + "/Meeting-4/detected-sequence-" +
    // to_string(loopVar + 1) + ".bmp", *(img + loopVar));
    // show the detected images
    imshow("detected circles - " + to_string(loopVar), *(img + loopVar));
  }

  //  waitKey();
}
