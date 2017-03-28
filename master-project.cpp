#include "basic.cpp"
#include "bottle-cap-detection.cpp"
#include "bottle-horiz-vert-detection.cpp"
#include "classes-operator-overload.cpp"
#include "classes-pointers.cpp"
#include "classes.cpp"
#include "discrete-fourier-transform.cpp"
#include "header.h"
#include "pointers.cpp"
#include "sobel-canny-threshold.cpp"
#include "stdafx.h"

string GetCWD(void) {
  char buff[FILENAME_MAX];
  GetCurrentDir(buff, FILENAME_MAX);
  string cwd(buff);
  return cwd;
}
Mat img;
Mat templ;
Mat result;
char* image_window = "Source Image";
char* result_window = "Result window";

int match_method;
int max_Trackbar = 5;
string masterproject::prjdir =
    "/Users/siddiqui/Documents/Projects/master-project/meetings/";

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod(int, void*) {
    /// Source image to display
    Mat img_display;
    img.copyTo(img_display);
    
    /// Create the result matrix
    int result_cols = img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    
    result.create(result_rows, result_cols, CV_32FC1);
    
    /// Do the Matching and Normalize
    matchTemplate(img, templ, result, match_method);
    normalize(result, result, 0, 100, NORM_MINMAX, -1, Mat());
    
    cout << "result: " << result << endl;
    /// Localizing the best match with minMaxLoc
    double minVal;
    double maxVal;
    Point minLoc;
    Point maxLoc;
    Point matchLoc;
    
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    
    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all
    /// the other methods, the higher the better
    if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED) {
        matchLoc = minLoc;
    } else {
        matchLoc = maxLoc;
    }
    

    cout << "match x: " << matchLoc.x << " match y: " << matchLoc.y << endl;
    /// Show me what you got
    rectangle(img_display, matchLoc,
              Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
              Scalar(0, 0, 255), 2, 8, 0);
    
    rectangle(result, matchLoc,
              Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
              Scalar(0, 0, 255), 2, 8, 0);
    
    imshow(image_window, img_display);
    imshow("template", templ);
    imshow(result_window, result);
    
    waitKey(0);
}

int main() {
  /// Load image and template
  img = imread(masterproject::prjdir + "meeting-11/doggy.jpg");
  templ = imread(masterproject::prjdir + "basic-bitch.png");

  /// Create windows
  namedWindow(image_window, CV_WINDOW_AUTOSIZE);
  namedWindow(result_window, CV_WINDOW_AUTOSIZE);

  /// Create Trackbar
  char* trackbar_label =
      "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM "
      "CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
  createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);

  MatchingMethod(0, 0);

  // detection of positions

  // without bottle
  //  Mat img2 = imread(masterproject::prjdir +
  //  "meeting-10/horizontal-perpendicular/52.bmp");

  BottleDetection detectInstance;
  // BottleDetection detectInstance2(masterproject::prjdir +
  // "meeting-10/no-bottle/7.bmp");

  // find peak signal to noise ratio
  // Mat image1 = imread(masterproject::prjdir + "meeting-11/misc/1.bmp");
  // Mat image2 = imread(masterproject::prjdir +
  // "meeting-11/horizontal-perpendicular/9.bmp");
  // detectInstance.getPSNR(image1, image2);

  //  detectInstance2.computeResults();
  // videoFrames(masterproject::prjdir + "meeting-10/meeting-10.mp4", true, 0,
  // 0);

  //    Mat img1 = imread(masterproject::prjdir  +
  //    "/meeting-9/without-label/projector/with-bottle-1-DFT.png");
  //    Mat img2 = imread(masterproject::prjdir  +
  //    "/meeting-9/without-label/projector/no-bottle-2-DFT.png");
  //    Mat diff;
  //    resize(img1, img1, Size(60, 900), 0,0, INTER_LINEAR);
  //    resize(img2, img2, Size(60, 900), 0,0, INTER_LINEAR);
  //    threshold(img1, img1, 25, 255, THRESH_BINARY);
  //    threshold(img2, img2, 25, 255, THRESH_BINARY);
  //    absdiff(img1, img2, diff);

  //    imshow("difference: ", img1);
  //    imshow("difference2: ", img2);
  //    imshow("difference_1: ", diff);
  // videoFrames(masterproject::prjdir + "/meeting-8/hh/hh-teach-video.mp4",
  // true, 35, 40);

  // detection of caps

  // videoFrames(masterproject::prjdir + "/meeting-5/cap-teach-video.mp4",
  // false, 30, 35);

  //  videoFrames(masterproject::prjdir + "/meeting-8/v/v-teach-video.mp4",
  //  false, 35, 40);

  // videoFrames(masterproject::prjdir + "/meeting-8/vh/vh-teach-video.mp4",
  // false, 35, 40);

  // videoFrames(masterproject::prjdir + "/meeting-8/vhv/vhv-teach-video.mp4",
  // false, 35, 45);
  return 0;
}
