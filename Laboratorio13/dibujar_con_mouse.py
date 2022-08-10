import cv2
import numpy as np

figure = 1
def draw_circle(event, x, y, flags, param):

    # Ejemplos de acciones con algunos eventos del mouse
    if event == cv2.EVENT_LBUTTONDOWN:
        if figure == 1:
            cv2.circle(imagen, (x, y), 20, (255, 0, 0), 2)
        elif figure == 2:
            cv2.rectangle(imagen, (x-18, y-18), (x+18, y+18), (255, 0, 0), 2)
        elif figure == 3:
            cv2.rectangle(imagen, (x-26, y-18), (x+26, y+18), (255, 0, 0), 2)
        elif figure == 4:
            pts = np.array([[x-18, y+18], [x+18, y+18], [x, y-18]])
            pts = pts.reshape(-1, 1, 2)
            cv2.polylines(imagen, [pts], True, (255, 0, 0), 2, cv2.LINE_AA)

imagen = np.zeros((600, 800, 3), np.uint8)
cv2.namedWindow('Imagen')
cv2.setMouseCallback('Imagen', draw_circle)
while True:
    cv2.imshow('Imagen', imagen)

    k = cv2.waitKey(1) & 0xFF
    if k == ord('1'):
        figure = 1
    elif k == ord('2'):
        figure = 2
    elif k == ord('3'):
        figure = 3
    elif k == ord('4'):
        figure = 4
    elif k == ord('l'):  # Limpiar el contenido de la imagen
        imagen = np.zeros((600, 800, 3), np.uint8)
    elif k == 27:
        break
cv2.destroyAllWindows()
