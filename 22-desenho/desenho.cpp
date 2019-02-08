#include "opencv2/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main(int, char**)
{
	Mat img(512, 512, CV_8UC3, Scalar(0, 0, 0));

	putText(img,
            "Um circulo",
            Point(130, 50),				// Coordenadas
            FONT_HERSHEY_SIMPLEX, 		// Fonte
            1.5, 						// Escala. 2.0 = 2x maior
            Scalar(255, 0, 0), 			// Cor
            2, 							// Espessura
            CV_AA); 					// Anti-alias

	line(img, Point(130, 60), Point(385, 60), Scalar(0, 255, 0), 2, LINE_AA);

	circle(img, Point(img.rows/2, img.cols/2), 150, Scalar(0, 255, 255), -1);
	imshow("Imagem", img);

    waitKey(0);
    return 0;
}
