#include "stdafx.h"
#include "header.h"

using namespace std;
using namespace cv;
void bottleCapDetection(void) {

	// array for images
	Mat img[5];

	int arraySize = sizeof(img) / sizeof(*img); // img/img[0]

	// reference the images in array
	for (int loopVar = 0; loopVar < arraySize; loopVar++) {
        *(img + loopVar) = imread(masterproject::prjdir + "/Meeting-4/sequence-" + to_string(loopVar) + ".bmp", CV_LOAD_IMAGE_COLOR);
	}

	// apply medianBlur, thresholding and houghcircle on each image
	for (int loopVar = 0; loopVar < arraySize; loopVar++) {

		// gray scale conversion
		Mat gray;
		cvtColor(*(img + loopVar), gray, COLOR_BGR2GRAY);

		// median filter to reduce noise
		medianBlur(gray, gray, 5);

		// threshold to make image binary
		// ! will not work in each scenario !
		threshold(gray, gray, 20, 255, THRESH_BINARY); // --> diable for cap.jpg

		// hough circles
		vector<Vec3f> circles;
		HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1,
			// change this value to detect circles with different distances to each other
			gray.rows / 8,
			// canny parameters
			200, 10,
			// min_radius & max_radius
			20, 40
		);

		/* radius params:
		 multicolor-bottle-caps.jpeg -- 40, 45
		 blob.jpg -- 30, 32
		 cap.jpg -- gray.rows/4, 82, 90
		 3-light-vertical.jpg -- gray.rows/8, 20, 40
		 4-light-angled.bmp -- 15, 25
		 transparent-glass-bottle.jpg -- gray.rows/4, 50, 60
		 glass-bottle.jpg -- 70, 75
		 cap-top-view.jpg -- threshold(gray, gray, 200, 255, THRESH_BINARY_INV); 30, 50
		 Wine-bottle.jpg -- threshold(gray, gray, 50, 255, THRESH_BINARY); 30, 40
		 */

		 // draw the caps
		
		for (size_t i = 0; i < circles.size(); i++) {
			Vec3i c = circles[i];
			 circle(*(img + loopVar), Point(c[0], c[1]), c[2], Scalar(0, 0, 255), 3, 8, 0);
		 	 circle(*(img + loopVar), Point(c[0], c[1]), 2, Scalar(0, 255, 0), 3, 8, 0);
		}
	}

	for (int loopVar = 0; loopVar < arraySize; loopVar++) {

		// save the detected images
	    imwrite(masterproject::prjdir + "/Meeting-5/detected-sequence-" + to_string(loopVar + 1) + ".bmp", *(img + loopVar));
		// show the detected images
		imshow("detected circles - " + to_string(loopVar), *(img + loopVar));
	}

	waitKey();
}