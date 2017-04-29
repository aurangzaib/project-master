import numpy as np
import cv2 as cv

# common parameters when
# no filter is used
bottle_no_filter = {
    "average_blob_area": 11.5,
    "min_threshold_value": 130,
    "filter_kernel_size": 5,
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
# placeholder for the parameter
bottle_flag = bottle_no_filter


class BottleDetection(object):
    def __init__(self, _image):
        """
        constructor
        """
        self.input_image = _image
        self.output_image = _image
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
        params = cv.SimpleBlobDetector_Params()
        params.filterByCircularity = False
        params.filterByConvexity = False
        params.filterByArea = False
        params.filterByInertia = False
        params.filterByColor = True
        params.blobColor = 255

        detector = cv.SimpleBlobDetector(params)
        key_points = detector.detect(self.output_image)
        height, width, channel = self.output_image.shape
        _size = 0
        unique_key_points = []
        for point in key_points:
            if point.size > bottle_flag["average_blob_area"]:
                x = point.pt[0]
                y = point.pt[1]
                x_boundary = x > bottle_flag["tolerance"] and abs(x - width) > bottle_flag["tolerance"]
                y_boundary = y > bottle_flag["tolerance"] and abs(y - width) > bottle_flag["tolerance"]
                if x_boundary and y_boundary:
                    unique_key_points.append(point)
                    _size += point.size
                cv.drawMarker(self.input_image,
                              (int(x), int(y)),
                              (0, 0, 255),
                              cv.MARKER_CROSS,
                              bottle_flag["marker_size"],
                              bottle_flag["marker_thickness"])
        _length = len(unique_key_points) if len(unique_key_points) > 0 else 1
        _size /= _length

    def get_dark_bottles(self):
        params = cv.SimpleBlobDetector_Params()
        params.filterByCircularity = False
        params.filterByConvexity = False
        params.filterByArea = False
        params.filterByInertia = False
        params.filterByColor = True
        params.blobColor = 255

        detector = cv.SimpleBlobDetector(params)
        key_points = detector.detect(self.output_image)
        height, width, channel = self.output_image.shape
        _size = 0
        unique_key_points = []
        for point in key_points:
            if point.size > bottle_flag["average_blob_area"]:
                x = point.pt[0]
                y = point.pt[1]
                x_boundary = x > bottle_flag["tolerance"] and abs(x - width) > bottle_flag["tolerance"]
                y_boundary = y > bottle_flag["tolerance"] and abs(y - width) > bottle_flag["tolerance"]
                if x_boundary and y_boundary:
                    unique_key_points.append(point)
                    _size += point.size
                cv.drawMarker(self.input_image,
                              (int(x), int(y)),
                              (0, 0, 255),
                              cv.MARKER_CROSS,
                              bottle_flag["marker_size"],
                              bottle_flag["marker_thickness"])
        _length = len(unique_key_points) if len(unique_key_points) > 0 else 1
        _size /= _length

    def bottle_detection(self):
        min_thresh_value = 150
        filter_kernel_size = 1
        self.output_image = self.reduce_image_density(self.output_image,
                                                      min_thresh_value,
                                                      filter_kernel_size)
        self.get_bottles()

    def compute_results(self, _with_wait_key=True):
        thresh = self.apply_filters(self.input_image)
        self.apply_probabilistic_hough_transform(thresh, _with_wait_key)

    @staticmethod
    def get_region_of_interest(reference_image, x, y, width, height):
        mask = np.zeros(reference_image.shape, np.uint8)
        mask[y:y + height, x:x + width] = reference_image[y:y + height, x:x + width]
        return mask

    @staticmethod
    def reduce_image_density(_image, _min_thresh_value, _filter_kernel_size):
        # _image = cv.medianBlur(_image, _filter_kernel_size)
        ret, _image = cv.threshold(
            _image, _min_thresh_value, 255, cv.THRESH_BINARY)
        # cv.imshow("reduces--bottle", _image)
        return _image
