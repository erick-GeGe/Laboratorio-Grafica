import cv2

image = cv2.imread('paisaje.jpg')

width = image.shape[1]
height = image.shape[0]
new_size = (int(width*0.8), int(height*0.8))

image = cv2.resize(image, new_size)

blue = image.copy()
blue[:, :, 1] = 0
blue[:, :, 2] = 0

cv2.imshow('Original image', image)
cv2.imshow('Blue image', blue)

cv2.waitKey(0)