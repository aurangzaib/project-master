import cv2 as cv
import os
import fnmatch
import requests
from numpy import fromstring, uint8
from bottle_detection import BottleDetection
from cap_detection import CapDetection

cwd = "/Users/siddiqui/Documents/Projects/master-project/meetings/"
ip_address = 'http://192.168.2.102:9090/'


def fetch_images_from_folder(folder):
    _images = []
    for root, dir_names, file_names in os.walk(folder):
        for filename in fnmatch.filter(file_names, '*.bmp'):
            img = cv.imread(os.path.join(folder, os.path.join(root, filename)))
            if img is not None:
                _images.append(img)
    return _images


def get_stream_from_ip():
    stream = requests.get(ip_address, stream=True)
    if stream.status_code == 200:
        data_bytes = bytes()
        for chunk in stream.iter_content(chunk_size=2048):
            data_bytes += chunk
            a = data_bytes.find(b'\xff\xd8')
            b = data_bytes.find(b'\xff\xd9')
            if a != -1 and b != -1:
                jpg = data_bytes[a:b + 2]
                data_bytes = data_bytes[b + 2:]
                frame = cv.imdecode(fromstring(jpg, dtype=uint8), cv.IMREAD_COLOR)
                perform_algorithm(frame)
                if cv.waitKey(1) == 27:
                    exit(0)


def perform_detection():
    images = fetch_images_from_folder(cwd + "algorithms-teach-data/")
    for image in images:
        perform_algorithm(image)
    cv.destroyAllWindows()


def perform_algorithm(image):
    image = cv.resize(image, None, fx=2, fy=2, interpolation=cv.INTER_LINEAR)
    height, width, channel = image.shape
    image = BottleDetection.get_region_of_interest(image,
                                                   width / 7,  # x
                                                   height / 5,  # y
                                                   width - (2 * width / 5),
                                                   height - height / 3)
    detect_cap = CapDetection(image)
    detect_bottle = BottleDetection(image)
    detect_dark_bottle = BottleDetection(image)

    detect_cap.cap_detection()
    detect_bottle.get_bottles()
    detect_dark_bottle.get_dark_bottles()
    cv.imshow("result", image)
    cv.waitKey(1)


perform_detection()
