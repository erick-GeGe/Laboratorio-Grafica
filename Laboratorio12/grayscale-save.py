import cv2

image = cv2.imread('paisaje.jpg')

width = image.shape[1]
height = image.shape[0]
new_size = (int(width*0.8), int(height*0.8))

image = cv2.resize(image, new_size)
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

cv2.imwrite('gray_scale.jpg', gray)
cv2.waitKey(0)