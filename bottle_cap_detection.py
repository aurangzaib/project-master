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

    def apply_hough_transform(self, thresh, with_wait_key):
        self.output_image = self.input_image.copy()
        hough_lines = cv.HoughLines(
            thresh,
            1,
            180 * (np.pi / 180),
            10,
            0,
            0
        )
        for hough_line in hough_lines:
            for rho, theta in hough_line:

                a = np.cos(theta)
                b = np.sin(theta)

                x0 = rho * a
                y0 = rho * b

                rows, columns, channels = self.input_image.shape
                pt1 = {
                    "x": int(np.round(x0 + int(rows) * (-b))),
                    "y": int(np.round(y0 + int(rows) * a))
                }

                pt2 = {
                    "x": int(np.round(x0 - int(rows) * (-b))),
                    "y": int(np.round(x0 - int(rows) * a))
                }

                cv.line(self.output_image,
                        (pt1['x'], pt1['y']),
                        (pt2['x'], pt2['y']),
                        (0, 0, 255),
                        2)

                cv.imshow("result", self.output_image)
                if with_wait_key:
                    cv.waitKey(1)

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
            cv.waitKey(1)

    @staticmethod
    def get_region_of_interest(reference_image, x, y, width, height):
        # not working correctly right now
        rec = reference_image[x:y, x+width:y+height]
        reference_image[x:y, x+width:y+width] = rec
        print rec
        cv.imshow("rect mask", rec)
        cv.waitKey(1)

    def compute_results(self, with_wait_key=True):
        thresh = self.apply_filters(self.input_image)
        self.apply_probabilistic_hough_transform(thresh, with_wait_key)
