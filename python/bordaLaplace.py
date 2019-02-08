import cv2

#cap = cv2.VideoCapture(1)
cap = cv2.VideoCapture('/home/unoesc/Videos/musica.mp4')

while True:
    _, frame = cap.read();

    lap = cv2.Laplacian(frame, cv2.CV_64FC3, 3)

    x = cv2.Sobel(frame, cv2.CV_64F, 1, 0, ksize=5)
    y = cv2.Sobel(frame, cv2.CV_64F, 0, 1, ksize=5)

    edges = cv2.Canny(frame, 200, 200)

    #cv2.imshow('Laplacian', lap)
    #cv2.imshow('X', x)
    #cv2.imshow('Y', y)
    cv2.imshow('Edges', edges)    

    k = cv2.waitKey(1)
    if k == 27:
        break

cv2.destroyAllWindows()
cap.release()