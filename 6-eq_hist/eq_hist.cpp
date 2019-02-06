#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
  Mat dst, src = imread("/home/debiasi/programas/octave/car.png");

  cvtColor(src, src, CV_BGR2GRAY); // Converte para escala de cinza

  // Aplica equalizacao do histograma
  equalizeHist(src, dst);

  imshow("Original", src);
  imshow("Equalizada", dst);

  waitKey(0);

  return 0;
}
