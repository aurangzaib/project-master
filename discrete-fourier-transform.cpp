#include "header.h"
#include "stdafx.h"

class discreteFourierTransform {
 private:
  string imagePath;
  Mat spatialImage, frequencyImage;

 public:
  // ctor default
  discreteFourierTransform();
  // ctor with image path
  discreteFourierTransform(const string);
  // ctor with image matrix
  discreteFourierTransform(const Mat spatialImage);
  // ctor declaration
  discreteFourierTransform(const string, const Mat, const Mat);
  // method to perform the DFT
  void performDFT(const bool acquireImage);
  // method to show the DFT
  void showDFTResult();
};

// ctor default
discreteFourierTransform::discreteFourierTransform() {
  Mat a, b;
  imagePath = "";
  spatialImage = a;
  frequencyImage = b;
}

// ctor default
discreteFourierTransform::discreteFourierTransform(const string imagePath)
    : imagePath(imagePath) {
  Mat a, b;
  spatialImage = a;
  frequencyImage = b;
}

// ctor default
discreteFourierTransform::discreteFourierTransform(const Mat spatialImage)
    : spatialImage(spatialImage) {
  Mat b;
  imagePath = "";
  frequencyImage = b;
}

// ctor definition
discreteFourierTransform::discreteFourierTransform(const string imagePath,
                                                   const Mat spatialImage,
                                                   const Mat frequencyImage)
    : imagePath(imagePath),
      spatialImage(spatialImage),
      frequencyImage(frequencyImage) {}

// perform method definition
void discreteFourierTransform::performDFT(const bool acquireImage) {
  // input image
  if (acquireImage == true)
    spatialImage =
        imread(masterproject::cwd + imagePath + ".png", CV_LOAD_IMAGE_COLOR);
  cout << "original image size: " << spatialImage.size() << endl;

  Mat gray;
  spatialImage.copyTo(gray);

  // reduce the noise
  blur(gray, gray, Size(3, 3));

  // transform image from 1 color system to another
  // transform from color to gray scale
  cvtColor(gray, gray, CV_BGR2GRAY);

  // perform thresholding
  // threshold(gray, gray, 20, 170, THRESH_BINARY);

  spatialImage = gray;
  // padding the border to optimize DFT
  int optimizedRows = getOptimalDFTSize(spatialImage.rows),
      optimizedCols = getOptimalDFTSize(spatialImage.cols);

  // create a new image with optimized borders
  Mat optimizedImage;
  copyMakeBorder(spatialImage,                       // source
                 optimizedImage,                     // destination
                 0,                                  // top
                 optimizedRows - spatialImage.rows,  // bottom
                 0,                                  // left
                 optimizedCols - spatialImage.cols,  // right
                 BORDER_CONSTANT, Scalar::all(0));

  // DFT results real and imaginary results
  // which have larger size than spatial coordinates
  Mat plane[] = {
      Mat_<float>(optimizedImage),               // element 1
      Mat::zeros(optimizedImage.size(), CV_32F)  // element 2
  };

  // create 1 multichannel array from several single channel array
  Mat dftImage;
  merge(plane,    // input arrays or matrices
        2,        // # of input arrays when input is plain array
        dftImage  // result
        );

  // apply dft
  dft(dftImage, dftImage);

  // split real and complex parts
  split(dftImage, plane);
  magnitude(plane[0],  // real part in  freq domain
            plane[1],  // imaginary part in freq domain
            plane[0]   // save magnitude in plane[0]
            );

  // save the magnitude of the image in Freq domain
  frequencyImage = plane[0];

  // save the result in log scale to better visualize it
  frequencyImage += Scalar::all(1);
  // crop odd rows or columns
  frequencyImage = frequencyImage(
      Rect(0, 0, frequencyImage.cols & -2, frequencyImage.rows & -2));

  // now we do dft shift to make center of image 0,0
  // in matlab its fft shift command
  int cx = frequencyImage.cols / 2;
  int cy = frequencyImage.rows / 2;

  Mat q0(frequencyImage,
         Rect(0, 0, cx, cy));  // Top-Left - Create a ROI per quadrant
  Mat q1(frequencyImage, Rect(cx, 0, cx, cy));   // Top-Right
  Mat q2(frequencyImage, Rect(0, cy, cx, cy));   // Bottom-Left
  Mat q3(frequencyImage, Rect(cx, cy, cx, cy));  // Bottom-Right

  Mat tmp;  // swap quadrants (Top-Left with Bottom-Right)
  q0.copyTo(tmp);
  q3.copyTo(q0);
  tmp.copyTo(q3);

  q1.copyTo(tmp);  // swap qua  drant (Top-Right with Bottom-Left)
  q2.copyTo(q1);
  tmp.copyTo(q2);

  // now we can normalize the image between 0 and 1
  normalize(frequencyImage,  // source
            frequencyImage,  // destination
            0,               // min range
            50,              // max range
            CV_MINMAX        // type of normalization
            );
  Mat resultImage = frequencyImage;
  if (false) {
    imwrite(masterproject::cwd + imagePath + "-DFT.png",
            resultImage);  // save as grey image
  }

  namedWindow(imagePath, WINDOW_NORMAL);
  imshow(imagePath, resultImage);
  //  waitKey();  // wait till key press to dismiss the window
}

void discreteFourierTransform::showDFTResult() {
  imshow("Image in spatial domain", spatialImage);  // spatial domain image
  imshow("Image in Freq domain", frequencyImage);   // freq domain image
  waitKey();  // wait till key press to dismiss the window
}
