#include "header.h"
#include "stdafx.h"

unsigned videoFrames(const string videoPath) {
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
    Mat capFrame, blobFrame;
    frame.copyTo(capFrame);
    frame.copyTo(blobFrame);
    BottleDetection detectBlob(blobFrame);
    CapDetection detect(capFrame, 30, 40);

    detect.applyHoughCircleTransform();
    // detectBlob.performBlobDetection();
  }
  return 0;
}
