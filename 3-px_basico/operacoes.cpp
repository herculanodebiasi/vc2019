#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat colorida = imread("/home/debiasi/Pictures/lena.png");

    Vec3b pixel=colorida.at<Vec3b>(100, 100);

    cout << "Pixel value (B,G,R): ("
         << (int)pixel[0] << ","
         << (int)pixel[1] << ","
         << (int)pixel[2] << ")"
         << endl;
}
