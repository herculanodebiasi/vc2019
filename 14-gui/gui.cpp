#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat im = cv::imread("/home/debiasi/Pictures/lena.png");
    if (im.empty())
    {
        cerr << "Cannot load image!" << endl;
        return -1;
    }

    //namedWindow("Lena", CV_GUI_NORMAL); //Ou CV_GUI_EXPANDED
    namedWindow("Lena", WINDOW_AUTOSIZE);
    //namedWindow("Lena", WINDOW_AUTOSIZE|WINDOW_OPENGL);
    moveWindow("Lena", 100, 100);

    blur(im, im, Size(20, 20));
    imshow("Lena", im);
    displayOverlay("Lena", "Overlay 5secs", 5000);
    waitKey(0);
}
