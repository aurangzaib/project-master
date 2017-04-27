import cv2 as cv
import os
import urllib
import numpy as np
from bottle_detection import BottleDetection
from cap_detection import CapDetection

cwd = "/Users/siddiqui/Documents/Projects/master-project/meetings/"


def fetch_images_from_folder(folder):
    _images = []
    for filename in os.listdir(folder):
        img = cv.imread(os.path.join(folder, filename))
        if img is not None:
            _images.append(img)
    return _images


def get_stream_from_ip():
    # Stream the video link
    stream = urllib.urlopen('http://141.60.168.144:8090/video?x.mjpeg')
    bytes = ''
    while True:
        bytes += stream.read(1024)
        # 0xff 0xd8 is the starting of the jpeg frame
        a = bytes.find('\xff\xd8')
        # 0xff 0xd9 is the end of the jpeg frame
        b = bytes.find('\xff\xd9')
        # Taking the jpeg image as byte stream
        if a != -1 and b != -1:
            os.system('clear')
            jpg = bytes[a:b + 2]
            bytes = bytes[b + 2:]
            # Decoding the byte stream to cv2 readable matrix format
            i = cv.imdecode(np.fromstring(jpg, dtype=np.uint8), 0)
            # Display
            # i = cv2.cvtColor(i, cv2.CV_BGR2HSV_FULL)
            _image = cv.medianBlur(i, 1)
            ret, _image = cv.threshold(_image, 100, 255, cv.THRESH_BINARY)
            cv.namedWindow('master project', flags=cv.WINDOW_NORMAL)
            cv.imshow('master project', _image)
            print ("Press 'q' to exit")
            # Exit key
            if cv.waitKey(1) & 0xFF == ord('q'):
                cv.destroyAllWindows()
                exit(0)


def perform_detection():
    images = fetch_images_from_folder(cwd + "meeting-12/lower/")
    for image in images:
        height, width, channels = image.shape
        image = BottleDetection.get_region_of_interest(image,
                                                       width / 20,  # x
                                                       height / 20,  # y
                                                       # width
                                                       width - (2 * width / 20),
                                                       height - height / 5)  # height
        detect_cap = CapDetection(image)
        detect_bottle = BottleDetection(image)
        detect_cap.cap_detection()
        detect_bottle.bottle_detection()
        cv.imshow("result", image)
        cv.waitKey()
    cv.destroyAllWindows()

perform_detection()