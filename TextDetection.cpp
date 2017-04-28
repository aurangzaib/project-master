#include "stdafx.h"
#include "header.h"


void textDetection() {

	Mat large = imread(prj::cwd + "/Meeting-6/rec1.jpg", CV_LOAD_IMAGE_COLOR);
	Mat rgb;
	// downsample and use it for processing
	pyrDown(large, rgb);
	
	Mat small;
	cvtColor(rgb, small, CV_BGR2GRAY);
	// morphological gradient
	Mat grad;
	Mat morphKernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	morphologyEx(small, grad, MORPH_GRADIENT, morphKernel);
	
	// binarize
	Mat bw;
	threshold(grad, bw, 0.0, 255.0, THRESH_BINARY | THRESH_OTSU);
	imshow("morphology", bw);
	/*Mat edge;
	Canny(bw,edge,50,100,3);
	imshow("edge detection ",edge);

	Mat sobelx;
	Sobel(bw, sobelx, CV_32F, 1, 0);
	imshow("sobel detection ", sobelx);*/
	// connect horizontally oriented regions
	Mat connected;
	morphKernel = getStructuringElement(MORPH_RECT, Size(9, 1));
	morphologyEx(bw, connected, MORPH_CLOSE, morphKernel);
	// find contours
	Mat mask = Mat::zeros(bw.size(), CV_8UC1);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(connected, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	// filter contours
	for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
	{
		Rect rect = boundingRect(contours[idx]);
		Mat maskROI(mask, rect);
		maskROI = Scalar(0, 0, 0);
		// fill the contour
		drawContours(mask, contours, idx, Scalar(255, 255, 255), CV_FILLED);
		// ratio of non-zero pixels in the filled region
		double r = (double)countNonZero(maskROI) / (rect.width*rect.height);

		if (r > .45 /* assume at least 45% of the area is filled if it contains text */
			&&
			(rect.height > 8 && rect.width > 8) /* constraints on region size */
												/* these two conditions alone are not very robust. better to use something
												like the number of significant peaks in a horizontal projection as a third condition */
			)
		{
			rectangle(rgb, rect, Scalar(0, 255, 0), 2);
		}
	}
	imshow("umair", rgb);
	imwrite(prj::cwd + "/Meeting-6/1-dark-grey.bmp", rgb);
	imwrite(prj::cwd + "/Meeting-6/1-dark-grey1.bmp", bw);
}