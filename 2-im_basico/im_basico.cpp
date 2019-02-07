#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <limits.h>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/core/types_c.h> 

using namespace cv;
using namespace std;

int main()
{
    Mat im = imread("/home/unoesc/Pictures/lena.png");

    cout << "Comprimento: " << im.size().width << endl;
    cout << "Altura.....: " << im.size().height << endl;

    cout << "\nLinhas.: " << im.rows << endl;
    cout << "Colunas: " << im.cols << endl;

    double min, max;
    minMaxLoc(im, &min, &max);
    cout << "\nMinimo: " << min << endl;
    cout << "Maximo: " << max << endl;

    cout << "\nProfundidade de um canal: ";
    if (im.depth() == CV_8U)
       cout << "CV_8U" << endl;

    cout << "Canais..................: " << im.channels() << endl;

    imshow("Imagem", im);
    waitKey(0);
}
