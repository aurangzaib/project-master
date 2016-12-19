#include "stdafx.h"
#include "header.h"

void laneDetection() {

	// http://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/hough_lines/hough_lines.html

	// input image
	Mat img = imread(masterproject::prjdir + "/Meeting-7/teach-snall-2-1.bmp", CV_LOAD_IMAGE_COLOR);

	// grayscale conversion
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY); // src, output, option

	// median blur to reduce the noise
	Mat median;
	medianBlur(
		gray,    // source
		median,  // destination
		3        // aperture size (odd and >1)
	);

	// canny contour detection
	Mat canny;
	Canny(
		median,   // source
		canny,    // destination
		120,       // low threshold
		200,      // high threshold
		3         // kernel size 3x3
	);

	// thesholding
	Mat thresh;
	threshold(
		median,
		thresh,
		80,
		255,
		THRESH_BINARY
	);

	// to save lines from hough transform
	vector <Vec2f> lines;

	// it will generate the accumulator cell
	// which is a 2xn matric contaning r->firstrow and theta->secondrow
	HoughLines(
		thresh,           // source
		lines,           // destination
		1,               // rho resolution
		270 * CV_PI / 180,       // theta resolution (1 degree here)
		0,             // min # of intersection to detect a line
		0, 0              // srn and stn
	);

	Mat detection;
	img.copyTo(detection);

	vector<int> line1Coordinates;
	vector<int> line2Coordinates;

	for (size_t loopVar = 0; loopVar < lines.size(); loopVar++) {

		// hough coordinates [row, theta]
		float rho = lines[loopVar][0]; // first row
		float theta = lines[loopVar][1]; // second row

		// cartesian coordinates [x, y]
		Point pt1, pt2;

		double a = cos(theta), b = sin(theta);

		double x0 = rho*a; // r * cos(theta)
		double y0 = rho*b; // r * sin(thera)

		// point 1
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));

		// point 2
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));

		line1Coordinates.push_back(pt1.x);
		line2Coordinates.push_back(pt2.x);

		// draw line using point1 and point2. red color
		line(detection, pt1, pt2, Scalar(0, 0, 255), 1, CV_AA);
		cout << "point1 : " << pt1 << endl << "point2: " << pt2 << endl << endl;
	}

	std::sort(line1Coordinates.begin(), line1Coordinates.end());
	
	vector<int> condensedArray;
	
	if (line1Coordinates.size() < 3) {
		condensedArray = line1Coordinates;
	}
	
	else {
		condensedArray.push_back(line1Coordinates.at(0));
		for (int loopVar = 0; loopVar < line1Coordinates.size(); loopVar++) {
			cout << "  |  " << line1Coordinates.at(loopVar);
			if (loopVar > 1 && line1Coordinates.at(loopVar) - line1Coordinates.at(loopVar - 1) > 14)
				condensedArray.push_back(line1Coordinates.at(loopVar+1));
		}
	}

	
	cout << endl << endl << "first element: " << condensedArray.at(0) << endl;
	cout << "second element: " << condensedArray.at(1) << endl;
	int width = abs(condensedArray.at(0) - condensedArray.at(1));

	cout << endl << endl << "width is found: " << width << endl;

	if (width < 25) {
		cout << "there is no bottle" << endl;
	}
	else if (width > 25 && width < 50) {
		cout << "bottle is horizontal" << endl;
	}
	else if (width > 180 && width < 255) {
		cout << "bottle is vertical" << endl;
	}
	else {
		cout << "system could not found the situation" << endl;
	}

	imshow("1-original image: ", img);
	imshow("2-gray image", gray);
	imshow("3-median blur image", median);
	imshow("4-canny contour image", canny);
	imshow("5-threshold image", thresh);
	imshow("6-hough line transform ", detection);

	waitKey();

}
