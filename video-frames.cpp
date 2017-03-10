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

//  namedWindow("myFrames",
//              CV_WINDOW_AUTOSIZE);  // create a window called "MyVideo"
//  namedWindow("oldFrames", CV_WINDOW_AUTOSIZE);

    for (int loop=0; loop < framesLength; loop++) {
    Mat frame;

    bool bSuccess = cap.read(frame);  // read a new frame from video

    if (!bSuccess) {
      cout << "Cannot read the frame from video file" << endl;
      break;
    }
    
    // array for images
    Mat img[12];
    img[0] = frame;

    bottleCapDetection(img);
    if (waitKey(30) == 27)  // wait for 'esc' key press for 30 ms. If 'esc' key
                            // is pressed, break loop
    {
      cout << "esc key is pressed by user" << endl;
      break;
    }
  }
  return 0;
}
