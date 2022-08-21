import cv2
import numpy as np
import imutils

# to get moving cars
cap = cv2.VideoCapture('input.mp4')
kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(5,5))
fgbg = cv2.createBackgroundSubtractorMOG2(history=500, varThreshold=0)

# to text
font1 = cv2.FONT_HERSHEY_PLAIN
fontScale = 1.2
grosorLetra = 2


while(True):
    ret, frame = cap.read()
    if not ret:
        print("end the video")
        break
    frame = imutils.resize(frame, width=600)

    # to indicate move
    color_area = (0, 255, 0)
    text_detect = 'NO DETECTADO'
    
    # area to detect in grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    area_to_detect = np.array([[int(frame.shape[1]*0.35), int(frame.shape[0]*0.6)], [int(frame.shape[1]*0.35), int(frame.shape[0])], [int(frame.shape[1]*0.99), int(frame.shape[0])], [int(frame.shape[1]*0.99), int(frame.shape[0]*0.6)]])
    area_text = np.array([[int(frame.shape[1]*0.07), int(frame.shape[0]*0.73)], [int(frame.shape[1]*0.07), int(frame.shape[0] * 0.85)], [int(frame.shape[1]*0.33), int(frame.shape[0] * 0.85)], [int(frame.shape[1]*0.33), int(frame.shape[0]*0.73)]])
    
    # to mask
    mask_white = np.zeros(shape=(frame.shape[:2]), dtype=np.uint8)
    cv2.drawContours(mask_white, [area_to_detect], -1, (255), -1)

    # get mask
    image_to_detect = np.zeros(shape=(frame.shape[:2]), dtype=np.uint8)
    image_to_detect = cv2.bitwise_and(mask_white,gray,image_to_detect)

    # clear moving area
    image_without_bg = np.zeros(shape=(frame.shape[:2]), dtype=np.uint8)
    image_without_bg = fgbg.apply(image_to_detect)
    image_without_bg = cv2.morphologyEx(image_without_bg, cv2.MORPH_CLOSE, kernel)
    image_without_bg = cv2.dilate(image_without_bg, None, iterations=2)
    _, image_without_bg = cv2.threshold(image_without_bg, 254, 255, cv2.THRESH_BINARY)

    # get moving cars
    contours = cv2.findContours(image_without_bg, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[0]
    for contour in contours:
        if cv2.contourArea(contour) > 3000:
            x, y, w, h = cv2.boundingRect(contour)
            cv2.rectangle(frame, (x,y), (x+w, y+h), (0,225,0),2)
            cv2.putText(frame, 'Auto',  (x,y-5), font1, fontScale, (0,225,0), grosorLetra, cv2.LINE_AA)
            color_area = (0, 0, 255)
            text_detect = 'DETECTADO'

    # draw rectangles and detect of detection
    cv2.drawContours(frame, [area_to_detect], -1, color_area, 2)    
    cv2.drawContours(frame, [area_text], -1, (0), -1)    
    cv2.putText(frame, text_detect,  (int(frame.shape[1]*0.08), int(frame.shape[0]*0.81)), font1, fontScale, color_area, grosorLetra, cv2.LINE_AA)

    # show windows
    cv2.imshow('Input', frame)
    cv2.imshow('Gray', gray)
    # cv2.imshow('Mask Empty', mask_white)
    cv2.imshow('Imagen to detect', image_to_detect)
    cv2.imshow('Wihtout background', image_without_bg)

    k = cv2.waitKey(1) & 0xFF
    if k == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()