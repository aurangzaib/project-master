#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d.hpp"

using namespace std;
using namespace cv;

void blobDetection (){
    
    // project directory
    string prjdir = "/Users/aurangzaib/Documents/Hochschule/Winter 2016/Projects/Master Project/";
    
    // image
    Mat src = imread( prjdir+"/Meeting-3/blob.jpg", IMREAD_GRAYSCALE );
    
    // Setup SimpleBlobDetector parameters.
    SimpleBlobDetector::Params params;
    
    // Change thresholds
    params.minThreshold = 0;
    params.maxThreshold = 250;
    
    // Filter by Area.
    params.filterByArea = false;
    params.minArea = 200;
    
    // Filter by Circularity
    params.filterByCircularity = true;
    params.minCircularity = 0.9; // circle
    
    // Filter by Convexity
    params.filterByConvexity = false;
    params.minConvexity = 0.87;
    
    // Filter by Inertia
    // low inertia -> ellipse
    // high inertia -> circle
    params.filterByInertia = true;
    params.minInertiaRatio = 0.01;
    
    // vector to store the blob
    vector<KeyPoint> keypoints;
    
    // Set up detector with params
    SimpleBlobDetector detector(params);
    
    // You can use the detector this way
    detector.detect(src, keypoints);
    
    Mat image_with_keypoints;
    drawKeypoints( src, keypoints, image_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    
    // Show blobs
    imshow("keypoints", image_with_keypoints );
    
    // keep window open until enter is pressed
    waitKey (0);
}