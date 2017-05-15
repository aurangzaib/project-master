import numpy as np
import cv2 as cv

# common parameters when
# no filter is used
bottle_no_filter = {
    "average_blob_area": 10.,
    "min_threshold_value": 200,
    "filter_kernel_size": 1,
    "marker_size": 15,
    "marker_thickness": 3,
    "tolerance": 40
}

# common parameters when
# (analyzer and polarizer) filter is used
bottle_with_filter = {
    "average_blob_area": 2.0,
    "min_threshold_value": 30,
    "filter_kernel_size": 1,
    "marker_size": 15,
    "marker_thickness": 3,
    "tolerance": 40
}

dark_bottle_filter = {
    "min_blob_area": 40.0,
    "max_blob_area": 200.0,
    "min_threshold_value": 60,
    "filter_kernel_size": 1,
    "marker_size": 15,
    "marker_thickness": 3,
    "tolerance": 40,
    "morph_size": 21
}

# placeholder for the parameter
bottle_flag = bottle_no_filter
dark_flag = dark_bottle_filter


class BottleDetection(object):
    def __init__(self, _image):
        """
        constructor
        """
        self.input_image = _image
        self.output_image = _image
        self.linePoints = []
        self.lineUniquePoints = []

    @staticmethod
    def get_region_of_interest(_reference_image, _x , _y, _width, _height):
        return _reference_image[int(_y):int(_y + _height), int(_x):int(_x + _width)]

    @staticmethod
    def reduce_density(_image, _min_thresh_value, _filter_kernel_size, _method, caller):
        _image = cv.medianBlur(_image, _filter_kernel_size)
        ret, _image = cv.threshold(_image, _min_thresh_value, 255, _method)
        # cv.imshow("reduces--bottle", _image)
        return _image

    def apply_filters(self, image):
        """
        apply different filters
        :param image: image on which filter to be applied
        :return: filtered image
        """
        gray = cv.cvtColor(self.input_image, cv.COLOR_BGR2GRAY)

        median = cv.medianBlur(gray, 7)

        ret, thresh = cv.threshold(median,
                                   bottle_flag["min_threshold_value"],
                                   255,
                                   cv.THRESH_BINARY)
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

                if with_wait_key:
                    cv.waitKey(1)

    def apply_probabilistic_hough_transform(self, thresh, with_wait_key):
        hough_lines = cv.HoughLinesP(
            thresh, 1, 180 * (np.pi / 180), 10, 10, 10)
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

        if with_wait_key:
            cv.waitKey(1)

    def get_bottles(self):
        self.output_image = self.reduce_density(self.output_image,
                                                bottle_flag["min_threshold_value"],
                                                bottle_flag["filter_kernel_size"],
                                                cv.THRESH_BINARY,
                                                "bottles")
        params = cv.SimpleBlobDetector_Params()
        params.filterByCircularity = False
        params.filterByConvexity = False
        params.filterByArea = False
        params.filterByInertia = False
        params.filterByColor = True
        params.blobColor = 255

        detector = cv.SimpleBlobDetector_create(params)
        key_points = detector.detect(self.output_image)
        height, width, channel = self.output_image.shape

        for point in key_points:
            if point.size > bottle_flag["average_blob_area"]:
                x = point.pt[0]
                y = point.pt[1]

                x_safe = x > bottle_flag["tolerance"] and abs(
                    x - width) > bottle_flag["tolerance"]

                y_safe = y > bottle_flag["tolerance"] and abs(
                    y - width) > bottle_flag["tolerance"]

                if x_safe and y_safe:
                    cv.drawMarker(self.input_image,
                                  (int(x), int(y)),
                                  (0, 0, 255),
                                  cv.MARKER_CROSS,
                                  bottle_flag["marker_size"],
                                  bottle_flag["marker_thickness"])

    def get_dark_bottles(self):
        self.output_image = self.reduce_density(self.output_image,
                                                dark_flag["min_threshold_value"],
                                                dark_flag["filter_kernel_size"],
                                                cv.THRESH_BINARY_INV,
                                                "dark-bottles")

        element = cv.getStructuringElement(
            cv.MORPH_RECT,
            (1 * dark_flag["morph_size"] + 1, 1 * dark_flag["morph_size"] + 1))

        for index in range(5):
            self.output_image = cv.morphologyEx(self.output_image, cv.MORPH_OPEN, element)

        params = cv.SimpleBlobDetector_Params()
        params.filterByCircularity = False
        params.filterByConvexity = False
        params.filterByArea = False
        params.filterByInertia = False
        params.filterByColor = True
        params.blobColor = 255
        # apply blob detection with the given criteria
        detector = cv.SimpleBlobDetector_create(params)
        # vector of keypoints
        key_points = detector.detect(self.output_image)
        height, width, channel = self.output_image.shape
        for p in key_points:
            lower_boundary = p.size > dark_flag["min_blob_area"]
            upper_boundary = p.size < dark_flag["max_blob_area"]
            if lower_boundary and upper_boundary:
                x = p.pt[0]
                y = p.pt[1]
                x_boundary = x > dark_flag["tolerance"] and abs(
                    x - width) > dark_flag["tolerance"]
                y_boundary = y > dark_flag["tolerance"] and abs(
                    y - width) > dark_flag["tolerance"]
                if x_boundary and y_boundary:
                    cv.drawMarker(self.input_image,
                                  (int(x), int(y)),
                                  (255, 0, 0),
                                  cv.MARKER_CROSS,
                                  dark_flag["marker_size"],
                                  dark_flag["marker_thickness"])

    def compute_results(self, _with_wait_key=True):
        thresh = self.apply_filters(self.input_image)
        self.apply_probabilistic_hough_transform(thresh, _with_wait_key)
