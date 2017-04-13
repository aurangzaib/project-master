from bottle_cap_detection import BottleDetection
import cv2 as cv


def perform_bottle_cap_detection(frame, radius_min, radius_max):
    img = []
    img[0] = frame


def perform_bottle_position_detection(frame):
    detect = BottleDetection(frame)
    with_wait_key = False
    detect.compute_results(with_wait_key)


class VideoFrames(object):
    @staticmethod
    def operation_on_video_frames(video_path, which, radius_min, radius_max):
        video_cap = cv.VideoCapture(video_path)
        success, image = video_cap.read()
        if not success:
            print "cannot open"
            return -1

        fps = video_cap.get(cv.CAP_PROP_FPS)
        frames_length = int(video_cap.get(cv.CAP_PROP_FRAME_COUNT))
        while frames_length:
            success, frame = video_cap.read()
            if not success:
                print "cannot read frame"
                return -1
            perform_bottle_position_detection(frame)
            cv.waitKey(1)