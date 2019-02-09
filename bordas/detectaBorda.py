import cv2

cap = cv2.VideoCapture('/home/unoesc/Videos/musica.mp4')

while True:
    _, frame = cap.read()

    bordas=cv2.Canny(frame, 200, 200)

    cv2.imshow('Bordas', bordas)

    tecla = cv2.waitKey(1)
    if tecla == 27:
        break

cv2.destroyAllWindows()
cap.release()