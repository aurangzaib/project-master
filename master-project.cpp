#include "stdafx.h"
#include "header.h"
#include "cap_detection.cpp"
string prj::siddiqui = "C:/Users/Siddiqui/Documents/Projects/master-project/algorithms-teach-data";
string prj::cwd = prj::siddiqui;

void fetchImagesFromFolder(vector<Mat>& data, const string path) {
	vector<String> fn;
	glob(path, fn, true);  // recurse
	for (size_t k = 0; k < fn.size(); ++k) {
		cv::Mat im = cv::imread(fn[k]);
		if (im.empty()) continue;  // only proceed if sucsessful
		data.push_back(im);
	}
}

void run() {
	vector<Mat> images;
	fetchImagesFromFolder(images, prj::cwd);
	for (auto& inputImage : images) {
		Size s1 = inputImage.size();
		BottleDetection regionOfInterest(inputImage);
		// remove the edges of the image before applying the algorithms
		regionOfInterest.getRegionOfInterest(
			inputImage,      // image
			s1.width / 4.5,    // remove 1/7th from left
			s1.height / 10,  // remove 1/10th from top
			s1.width - (2 * s1.width / 9), s1.height - s1.height / 4);
		Mat capData, blobData;
		if (BY_REFERENCE) {
			capData = inputImage;
			blobData = inputImage;
		}
		else if (!BY_REFERENCE) {
			inputImage.copyTo(capData);
			inputImage.copyTo(blobData);
		}
		// instance of cap detection class
		CapDetection detectCaps(capData, 45, 65);
		// instance of bottle detection class
		BottleDetection detectBottles(blobData);
		// detect caps of the bottle
		detectCaps.getCaps();
		// detect presence of the bottle
		detectBottles.getBottles();
		// detect dark bottles
		if (NO_FILTER) detectBottles.getDarkBottles();
		// use images by address
		if (BY_REFERENCE) {
			if (ENLARGE_RESULTS)
				resize(inputImage, inputImage, Size(), 2.3, 2.3, INTER_LINEAR);
			if (SHOW_RESULT) imshow("results: ", inputImage);
			if (SAVE_RESULTS)
				::saveImage(prj::cwd + "/results/result.bmp", inputImage);
		}
		// use images by copy
		else if (!BY_REFERENCE) {
			Mat result;
			vconcat(blobData, capData, result);
			hconcat(blobData, capData, result);
			if (ENLARGE_RESULTS)
				resize(inputImage, inputImage, Size(), 2.3, 2.3, INTER_LINEAR);
			if (SAVE_RESULTS)
				::saveImage(prj::cwd + "/meeting-15/results/result.bmp", result);
			if (SHOW_RESULT) imshow("results: ", result);
		}
		// keep images showing
		saveImage(prj::cwd + "/blob-detection-cap.bmp", inputImage);
		waitKey(2);
	}
}


int main() {
	run();
	return 0;
}
