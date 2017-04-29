import cv2 as cv
import os
import fnmatch
import urllib
import numpy as np
from bottle_detection import BottleDetection
from cap_detection import CapDetection

cwd = "/Users/siddiqui/Documents/Projects/master-project/meetings/"


def fetch_images_from_folder(folder):
    _images = []
    for root, dir_names, file_names in os.walk(folder):
        for filename in fnmatch.filter(file_names, '*.bmp'):
            img = cv.imread(os.path.join(folder, os.path.join(root, filename)))
            if img is not None:
                _images.append(img)
    return _images


def get_stream_from_ip():
    # Stream the video link
    stream = urllib.urlopen('http://141.60.168.144:8090/video?x.mjpeg')
    image_bytes = ''
    while True:
        image_bytes += stream.read(1024)
        # 0xff 0xd8 is the starting of the jpeg frame
        a = image_bytes.find('\xff\xd8')
        # 0xff 0xd9 is the end of the jpeg frame
        b = image_bytes.find('\xff\xd9')
        # Taking the jpeg image as byte stream
        if a != -1 and b != -1:
            os.system('clear')
            jpg = image_bytes[a:b + 2]
            image_bytes = image_bytes[b + 2:]
            # Decoding the byte stream to cv2 readable matrix format
            i = cv.imdecode(np.fromstring(jpg, dtype=np.uint8),  # image type
                            -1)  # >0 3-channel, =0 gray scale, <0 as is it
            perform_algorithm(i)
            if cv.waitKey(1) & 0xFF == ord('q'):
                cv.destroyAllWindows()
                exit(0)


def perform_detection():
    images = fetch_images_from_folder(cwd + "meeting-15/")
    for image in images:
        perform_algorithm(image)
    cv.destroyAllWindows()


def perform_algorithm(image):
    image = cv.resize(image, None, fx=2, fy=2, interpolation=cv.INTER_LINEAR)
    height, width, channel = image.shape
    image = BottleDetection.get_region_of_interest(image,
                                                   width / 7,  # x
                                                   height / 5,  # y
                                                   # width
                                                   width - (2 * width / 5),
                                                   height - height / 3)  # height
    detect_cap = CapDetection(image)
    detect_bottle = BottleDetection(image)
    detect_dark_bottle = BottleDetection(image)

    detect_cap.cap_detection()
    detect_bottle.get_bottles()
    detect_dark_bottle.get_dark_bottles()

    cv.imshow("result", image)
    cv.waitKey(1)


get_stream_from_ip()
