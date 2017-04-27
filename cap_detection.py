import numpy as np
import cv2 as cv
from bottle_detection import BottleDetection


class CapDetection(object):
    def __init__(self, _image, min_radius=45, max_radius=60):
        self.input_image = _image
        self.output_image = _image
        self.min_radius = min_radius
        self.max_radius = max_radius

    def reduce_image_density(self):
        self.output_image = cv.cvtColor(self.output_image, cv.COLOR_BGR2GRAY)
        min_thresh_value = 5
        filter_kernel_size = 49
        self.output_image = BottleDetection.reduce_image_density(self.output_image,
                                                                 min_thresh_value,
                                                                 filter_kernel_size)

    def get_caps_using_blobs(self):
        params = cv.SimpleBlobDetector_Params()
        params.filterByArea = False
        params.filterByCircularity = False
        params.filterByConvexity = True
        params.minConvexity = 0.95
        params.maxConvexity = 1.0
        params.filterByInertia = True
        params.minInertiaRatio = 0.6
        params.maxInertiaRatio = 1
        params.filterByColor = True
        params.blobColor = 0

        detector = cv.SimpleBlobDetector(params)
        key_points = detector.detect(self.output_image)

        min_area = 30
        max_area = 150

        unique_key_points = []

        for point in key_points:
            if min_area < point.size < max_area:
                unique_key_points.append(point)
                self.input_image = cv.drawMarker(self.input_image,
                                                 (int(point.pt[0]),
                                                  int(point.pt[1])),
                                                 (255, 255, 255),
                                                 cv.MARKER_CROSS,
                                                 10,
                                                 3)

    def cap_detection(self):
        self.reduce_image_density()
        self.get_caps_using_blobs()
