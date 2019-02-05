#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat im = imread("/home/unoesc/Pictures/lena.jpg");
    if (im.empty()) 
    {
        cout << "Erro!" << endl;
        return -1;
    }
    imshow("Imagem", im);
    waitKey(0);
}

