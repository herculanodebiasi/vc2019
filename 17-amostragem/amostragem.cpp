#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main ()
{
    Mat image = cv::imread("landscape.jpg");

    for(int i = 1; i <= 4; i++) {
        string imagem("Tamanho: ");
	imagem += to_string(image.cols) + "x" + to_string(image.rows);
        imshow(imagem, image);
	resize(image, image, Size(image.cols/2, image.rows/2));

	imagem += ".png";
        imwrite(imagem, image);
    }

    waitKey(0);
    return 0;
}

