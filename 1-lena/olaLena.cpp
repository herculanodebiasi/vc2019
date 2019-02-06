#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main()
{
    cv::Mat im = cv::imread("/home/debiasi/Pictures/lena.png");
    if (im.empty())
    {
        std::cerr << "Cannot load image!" << std::endl;
        return -1;
    }
    //cv::namedWindow("Janela");
    cv::imshow("Imagem", im);
    cv::waitKey(0);
}
