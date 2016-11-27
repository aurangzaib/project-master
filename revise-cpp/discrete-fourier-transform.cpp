#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void discreteFourierTransform() {
    // project directory
    string prjdir = "/Users/aurangzaib/Documents/Hochschule/Winter "
                    "2016/Projects/Master Project/";

    // input image
    Mat img = imread(prjdir + "/Meeting-4/road-0.jpg", IMREAD_GRAYSCALE);
    cout << "original image size: " << img.size() << endl;
    // padding the border to optimize DFT
    int optimizedRows = getOptimalDFTSize(img.rows),
        optimizedCols = getOptimalDFTSize(img.cols);

    // create a new image with optmizied borders
    Mat optimizedImage;
    copyMakeBorder(img,                      // source
                   optimizedImage,           // destination
                   0,                        // top
                   optimizedRows - img.rows, // bottom
                   0,                        // left
                   optimizedCols - img.cols, // right
                   BORDER_CONSTANT);

    cout << "optimized image size: " << optimizedImage.size() << endl;

    // DFT results real and imaginary results
    // which have larger size than spatial coordinates
    Mat plane[] = {
        Mat_<float>(optimizedImage),              // element 1
        Mat::zeros(optimizedImage.size(), CV_32F) // element 2
    };

    cout << "plane size: " << sizeof (plane)/sizeof(plane[0]) << endl;

    // create 1 multichannel array from several single channel array
    Mat dftImage;
    merge(plane, 2, dftImage);

    // apply dft
    dft(dftImage, dftImage);
    cout << "dftImage size: " << dftImage.size() << endl;
    
    // split real and complex parts
    split(dftImage, plane);

    cout << "split real and imaginary size: " << sizeof (plane)/sizeof(plane[0])
         << "x" << sizeof(plane[0]) / sizeof(int)  << endl;
}