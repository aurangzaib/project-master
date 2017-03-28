#include "header.h"
#include "stdafx.h"

void performBottleCapDetection(Mat frame, const unsigned radiusMin,
                               const unsigned radiusMax) {
  // array for images
  Mat img[12];
  img[0] = frame;
  // frame and min, max radii
  bottleCapDetection(img, radiusMin, radiusMax);
}

void performBottlePositionDetection(Mat frame) {
  BottleDetection detect(frame);
  detect.computeResults();
}

unsigned videoFrames(const string videoPath, const bool which,
                     const unsigned radiusMin, const unsigned radiusMax) {
  // open the video file for reading
  VideoCapture cap(videoPath);
  // if not success, exit program
  if (!cap.isOpened()) {
    cout << "Cannot open the video file" << endl;
    return -1;
  }

  // cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

  // get the frames per seconds of the video
  double fps = cap.get(CV_CAP_PROP_FPS);
  // number of frames in the video
  double framesLength = cap.get(CV_CAP_PROP_FRAME_COUNT);
  cout << "Frame per seconds : " << fps << endl;
  cout << "Number of Frame : " << framesLength << endl;

  for (int loop = 0; loop < framesLength; loop++) {
    Mat frame;

    bool bSuccess = cap.read(frame);  // read a new frame from video

    if (!bSuccess) {
      cout << "Cannot read the frame from video file" << endl;
      break;
    }

    if (which == true)
      // find position of the bottle (horizontal/vertical)
      performBottlePositionDetection(frame);

    else if (which == false)
      // find position of the cap
      performBottleCapDetection(frame, radiusMin, radiusMax);

    // perform DFT on the frame

    discreteFourierTransform obj(frame);
    obj.performDFT(false);

    // wait for 'esc' key press for 30 ms. If 'esc' key
    // is pressed, break loop
    if (waitKey(30) == 27) {
      cout << "esc key is pressed by user" << endl;
      break;
    }
  }
  return 0;
}
