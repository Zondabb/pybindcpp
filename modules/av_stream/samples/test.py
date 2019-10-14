import sys
sys.path.insert(0, '/your/opencv-compile/lib/python3.7/site-packages')
import cv2
cap = cv2.av_stream_VideoCapture()
cap.open("video rtsp/rtmp url")

cnt = 0
while True:
    ret = cap.grab()
    # ret, frame = cap.read()
    if not ret:
        continue
    ret, frame = cap.retrieve()
    print (frame.shape)