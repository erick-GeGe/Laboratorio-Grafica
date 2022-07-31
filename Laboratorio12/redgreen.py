import cv2

image = cv2.imread('paisaje.jpg')

width = image.shape[1]
height = image.shape[0]
new_size = (int(width*0.8), int(height*0.8))

image = cv2.resize(image, new_size)

red_green = image.copy()
red_green[:, :,0] = 0

cv2.imwrite('red_green.jpg', red_green)
cv2.waitKey(0)