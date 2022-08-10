import cv2
import numpy as np

image = cv2.imread("image01.jpeg")

width = image.shape[1]
height = image.shape[0]
new_size = (int(width*0.8), int(height*0.8))
image = cv2.resize(image, new_size)

font1 = cv2.FONT_HERSHEY_PLAIN
person = 'PERSONA'
cat = 'GATO'
dog = 'PERRO'
fontScale = 1.5
grosorLetra = 2

# Person
pts = np.array([[400,280],[325,140],[450,50],[575,140],[500,280]])
pts = pts.reshape(-1,1,2)
image = cv2.polylines(image,[pts],True,(0,0,255), 2, cv2.LINE_AA)
cv2.putText(image, person,  (400, 310), font1, fontScale, (0,0, 255), grosorLetra, cv2.LINE_AA)

#  Cat
pts = np.array([[190,440],[140,330],[240,270],[340,330],[290,440]])
pts = pts.reshape(-1,1,2)
image = cv2.polylines(image,[pts],True,(0,0,255), 2, cv2.LINE_AA)
cv2.putText(image, cat,  (200, 470), font1, fontScale, (0,0, 255), grosorLetra, cv2.LINE_AA)

# Dog
pts = np.array([[580,320],[535,210],[630,150],[725,210],[680,320]])
pts = pts.reshape(-1,1,2)
image = cv2.polylines(image,[pts],True,(0,0,255), 2, cv2.LINE_AA)
cv2.putText(image, dog,  (590, 350), font1, fontScale, (0,0, 255), grosorLetra, cv2.LINE_AA)

cv2.imshow('original Image', image)

cv2.waitKey(0)
cv2.destroyAllWindows()

