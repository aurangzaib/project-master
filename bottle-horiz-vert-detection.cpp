#include "stdafx.h"
#include "header.h"

void bottleHorizVertDetection(void) {

    // input image
	Mat img = imread(masterproject::prjdir + "/Meeting-7/original-2.bmp", CV_LOAD_IMAGE_COLOR);

	// grayscale conversion
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);                                        // src, output, option

	// median blur to reduce the noise
	Mat median;
	medianBlur(
		gray,                                                                   // source
		median,                                                                 // destination
		3                                                                       // aperture size (odd and >1)
	);

	// canny contour detection
	Mat canny;
	Canny(
		median,                                                                 // source
		canny,                                                                  // destination
		120,                                                                    // low threshold
		200,                                                                    // high threshold
		3                                                                       // kernel size 3x3
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
		thresh,                                                                 // source
		lines,                                                                  // destination
		1,                                                                      // rho resolution
		180*CV_PI / 180,                                                      // theta resolution (1 degree here)
		10,                                                                     // min # of intersection to detect a line
		0, 0                                                                    // srn and stn
	);

	Mat detection;
	img.copyTo(detection);

	vector<int> lineCoordinates;

	for (size_t loopVar = 0; loopVar < lines.size(); loopVar++) {

		// hough coordinates [row, theta]
		float rho = lines[loopVar][0];                                          // first row
		float theta = lines[loopVar][1];                                        // second row
		Point pt1, pt2;                                                         	// cartesian coordinates [x, y]

		double a = cos(theta), b = sin(theta);

		double x0 = rho*a;                                                      // r * cos(theta)
		double y0 = rho*b;                                                      // r * sin(thera)

		// point 1
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));

		// point 2
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));

		lineCoordinates.push_back(pt1.x);                                       // push point1 coords
        line(detection, pt1, pt2, Scalar(0, 0, 255), 1, CV_AA);                 // draw line using point1 and point2. red color
	}

	std::sort(lineCoordinates.begin(), lineCoordinates.end(), std::greater<int>());  // sort in descending order
	
	vector<int> condensedArray;
	
    for (int loopVar = 0; loopVar < lineCoordinates.size(); loopVar++){
        cout << "value: " << lineCoordinates.at(loopVar) << endl;
    }
    
	if (lineCoordinates.size() < 3) {
		condensedArray = lineCoordinates;
	}
	
	else {
        size_t valueRange = lineCoordinates.at(0) - lineCoordinates.at(lineCoordinates.size()-1);
        cout << endl;
        condensedArray.push_back(lineCoordinates.at(0));
		for (int loopVar = 0; loopVar < lineCoordinates.size()-1; loopVar++) {
			cout << endl << "comparing " << lineCoordinates.at(loopVar) << " with " << lineCoordinates.at(loopVar+1);
            if ((lineCoordinates.at(loopVar) - lineCoordinates.at(loopVar + 1)) > valueRange/2)
                condensedArray.push_back(lineCoordinates.at(loopVar+1));
		}
	}

    cout << endl << endl << "value selected" << endl;
	for (int loopVar = 0; loopVar < condensedArray.size(); loopVar++) {
        cout << condensedArray.at(loopVar) << " ";
    }

    int width = abs(condensedArray.at(0) - condensedArray.at(1));
	cout << endl << endl << "width is found: " << width << endl;

	if (width < 5) {
		cout << "there is no bottle" << endl;
	}
	else if (width > 5 && width < 80) {
		cout << "bottle is horizontal" << endl;
	}
	else if (width >= 80 && width < 255) {
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
