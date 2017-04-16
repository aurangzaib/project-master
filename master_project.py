import cv2 as cv
import os
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


images = fetch_images_from_folder(cwd + "meeting-14/")
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
    cv.waitKey(500)

cv.destroyAllWindows()
