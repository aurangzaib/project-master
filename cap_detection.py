import numpy as np
import cv2 as cv
from bottle_detection import BottleDetection

cap_no_filter = {
    "min_convex": 0.75,
    "min_inertia_ratio": 0.55,
    "min_threshold_value": 15,
    "filter_kernel_size": 7,
    "marker_size": 12,
    "marker_width": 2,
    "min_area": 40,
    "max_area": 100
}

cap_with_filter = {
    "min_convex": 0.90,
    "min_inertia_ratio": 0.75,
    "min_threshold_value": 5,
    "filter_kernel_size": 49,
    "marker_size": 12,
    "marker_width": 2,
    "min_area": 40,
    "max_area": 100
}

cap_flag = cap_no_filter


class CapDetection(object):
    def __init__(self, _image, min_radius=45, max_radius=60):
        self.input_image = _image
        self.output_image = _image
        self.min_radius = min_radius
        self.max_radius = max_radius

    def reduce_density(self):
        # self.output_image = cv.cvtColor(self.output_image, cv.COLOR_BGR2GRAY)
        self.output_image = BottleDetection.reduce_density(self.output_image,
                                                           cap_flag["min_threshold_value"],
                                                           cap_flag["filter_kernel_size"],
                                                           cv.THRESH_BINARY,
                                                           "cap")

    def get_caps_using_blobs(self):
        params = cv.SimpleBlobDetector_Params()
        params.filterByArea = False
        params.filterByCircularity = False
        # high convexity i.e. no breakage in the shape
        # near to the circle
        params.filterByConvexity = True
        params.minConvexity = cap_flag["min_convex"]
        params.maxConvexity = 1.0
        # high inertia i.e. blob should not be
        # elongated but it should be near circle shape
        params.filterByInertia = True
        params.minInertiaRatio = cap_flag["min_inertia_ratio"]
        params.maxInertiaRatio = 1
        # filter blob based on black colors
        # threshold is applied in a way
        # that it makes caps as black (0,0,0)
        params.filterByColor = True
        params.blobColor = 0

        detector = cv.SimpleBlobDetector(params)
        key_points = detector.detect(self.output_image)

        unique_key_points = []

        for point in key_points:
            if cap_flag["min_area"] < point.size < cap_flag["max_area"]:
                unique_key_points.append(point)
                cv.drawMarker(self.input_image,
                              (int(point.pt[0]),
                               int(point.pt[1])),
                              (0, 255, 0),
                              cv.MARKER_CROSS,
                              cap_flag["marker_size"],
                              cap_flag["marker_width"])
        # draw caps points -- circle
        cv.drawKeypoints(self.input_image,  # input image
                         unique_key_points,  # keypoints found using blob detection
                         self.input_image,  # output image
                         (0, 255, 0),  # colour for the points
                         cv.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

    def cap_detection(self):
        self.reduce_density()
        self.get_caps_using_blobs()
