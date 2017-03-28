import numpy as np
import cv2 as cv


class BottleDetection(object):
    def __init__(self, input_image):
        """
        constructor
        """
        self.input_image = input_image
        self.output_image = input_image
        self.linePoints = []
        self.lineUniquePoints = []

    def apply_filters(self, image):
        """
        apply different filters
        :param image: image on which filter to be applied 
        :return: filtered image
        """
        gray = cv.cvtColor(self.input_image, cv.COLOR_BGR2GRAY)

        median = cv.medianBlur(gray, 7)

        canny = cv.Canny(median, 120, 200, 3)
        ret, thresh = cv.threshold(median, 170, 255, cv.THRESH_BINARY)

        return thresh

    def apply_hough_transform(self, thresh):
        self.input_image.copyTo(self.output_image)
        hough_lines = cv.HoughLines(
            thresh,
            1,
            180 * (np.pi / 180),
            10,
            0,
            0
        )

        for line in hough_lines:
            rho = line[0]
            theta = line[1]

            a = np.cos(theta)
            b = np.sin(theta)

            x0 = rho * a
            y0 = rho * b

            pt1 = {
                "x": np.round(x0 + self.input_image.rows * (-b)),
                "y": np.round(y0 + self.input_image.rows * (a))
            }

            pt2 = {
                "x": np.round(x0 - self.input_image.rows * (-b)),
                "y": np.round(x0 - self.input_image.rows * (a))
            }

            cv.line(self.output_image, pt1, pt2, (0, 0, 255), 1)

    def apply_probabilistic_hough_transform(self, thresh, with_wait_key):
        hough_lines = cv.HoughLinesP(thresh, 1, 180 * (np.pi / 180), 10, 10, 10)
        self.output_image = self.input_image.copy()
        for hough_line in hough_lines:
            for x1, y1, x2, y2 in hough_line:
                cv.line(
                    self.output_image,
                    (x1, y1),
                    (x2, y2),
                    (0, 0, 255),
                    1
                )

        cv.imshow("result", self.output_image)
        if with_wait_key:
            cv.waitKey()

    def compute_results(self, with_wait_key=True):
        thresh = self.apply_filters(self.input_image)
        self.apply_probabilistic_hough_transform(thresh, with_wait_key)
