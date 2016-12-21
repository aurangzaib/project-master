#include "stdafx.h"
#include "header.h"

void sobelCannyThreshold() {
  Mat src, grey, sobelx, canny, thresholdImage, draw;
  double minValue, maxValue;

  src = imread(masterproject::prjdir + "/Meeting-2/1-dark.bmp",
               CV_LOAD_IMAGE_COLOR);
  // reduce noise with 3x3 kernel
  blur(src, src, Size(3, 3));
  // transform image from 1 color system to another
  cvtColor(src, grey, CV_BGR2GRAY);
  // 3x3 sobelx
  Sobel(grey, sobelx, CV_32F, 1, 0, 3);
  // find max and min in the image
  minMaxLoc(grey, &minValue, &maxValue);
  // make sobelx dense, initially its sparse matrix
  sobelx.convertTo(draw, CV_32F);
  // perform thresholding
  threshold(grey, thresholdImage, 20, 170, THRESH_BINARY);
  // perform canny edge detector
  Canny(grey, canny, 0, 180, 3);

  // max and min values of grey scale image
  std::cout << "min value: " << minValue << " max value: " << maxValue << endl;

  // show images
  imshow("Original image", src);
  imshow("greyscale image", grey);
  imshow("sobel x image", sobelx);
  imshow("canny edge image", canny);
  imshow("threshold image", thresholdImage);

  // write to disk
  imwrite(masterproject::prjdir + "/Meeting-2/1-dark-grey.bmp",
          grey);  // save as grey image
  imwrite(masterproject::prjdir + "/Meeting-2/1-dark-grey.bmp",
          sobelx);  // save as grey image
  imwrite(masterproject::prjdir + "/Meeting-2/1-dark-grey.bmp",
          canny);  // save as grey image
  imwrite(masterproject::prjdir + "/Meeting-2/1-dark-threshold.bmp",
          thresholdImage);  // save as grey image
  // keep window open until enter is pressed
  waitKey(0);
}