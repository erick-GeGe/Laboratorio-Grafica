import cv2

image = cv2.imread("image02.jpeg")
width = image.shape[1]
height = image.shape[0]
new_size = (int(width*0.8), int(height*0.8))
image = cv2.resize(image, new_size)

font1 = cv2.FONT_HERSHEY_PLAIN
person = 'PERSONA'
cat = 'GATO'
dog = 'PERRO'
fontScale = 0.8
grosorLetra = 2

# Person 1
image = cv2.circle(image, (125, 50), 35, (0, 0, 255), 2, cv2.LINE_AA)
cv2.putText(image, person,  (85, 98), font1, fontScale, (0, 0, 0), grosorLetra, cv2.LINE_AA)
# Dog 1
image = cv2.circle(image, (200, 50), 40, (0, 0, 255), 2, cv2.LINE_AA)
cv2.putText(image, dog,  (180, 100), font1, fontScale, (0, 0, 0), grosorLetra, cv2.LINE_AA)
# Person 2
image = cv2.circle(image, (250, 125), 35, (0, 0, 255), 2, cv2.LINE_AA)
cv2.putText(image, person,  (230, 170), font1, fontScale, (0, 0, 0), grosorLetra, cv2.LINE_AA)
# Person 2
image = cv2.circle(image, (355, 95), 35, (0, 0, 255), 2, cv2.LINE_AA)
cv2.putText(image, person,  (335, 140), font1, fontScale, (0, 0, 0), grosorLetra, cv2.LINE_AA)
# Cat 1
image = cv2.circle(image, (380, 170), 30, (0, 0, 255), 2, cv2.LINE_AA)
cv2.putText(image, cat,  (360, 215), font1, fontScale, (0, 0, 0), grosorLetra, cv2.LINE_AA)

cv2.imshow('original Image', image)
cv2.waitKey(0)
cv2.destroyAllWindows()
