import sys
sys.path.insert(0, '/your/opencv-compile/lib/python3.7/site-packages')
import cv2

img = cv2.imread('test.jpg')
model = cv2.dnn_inference_Model()
model.open('test.model')
rects = model.face_detect(img)
ret, pts = model.landmark(img, rects.tolist())