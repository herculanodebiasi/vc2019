#include "opencv2/opencv.hpp"

using namespace cv;

int main(int argc, char** argv)
{
  Mat img, gray;

  img = imread(argv[1], 1);
  imshow("original", img);

  cvtColor(img, gray, COLOR_BGR2GRAY);
  GaussianBlur(gray, gray, Size(11, 11), 10);
  Canny(gray, gray, 0, 50);

  imshow("edges", gray);
  waitKey();
  return 0;
}
