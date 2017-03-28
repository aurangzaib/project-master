import cv2 as cv
from video_frames import VideoFrames
prj_dir = "/Users/siddiqui/Documents/Projects/master-project/meetings/"
input_image = cv.imread(prj_dir + "meeting-10/horizontal-perpendicular/52.bmp")

VideoFrames.operation_on_video_frames(prj_dir + "meeting-10/meeting-10.mp4", True, 0,0)
cv.waitKey()