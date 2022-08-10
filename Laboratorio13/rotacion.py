import cv2
import numpy as np

image = cv2.imread("image02.jpeg")

width = image.shape[1]
height = image.shape[0]
new_size = (int(width*0.8), int(height*0.8))
image = cv2.resize(image, new_size)

image_norm = cv2.rotate(image, cv2.ROTATE_90_CLOCKWISE)

cv2.imshow('original Image', image)
cv2.imshow('Rotated Image', image_norm)
cv2.waitKey(0)
cv2.destroyAllWindows()
