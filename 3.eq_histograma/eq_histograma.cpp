#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp> 
#include <iostream> 
#include <stdio.h> 

using namespace cv; 
using namespace std; 

int main( int argc, char** argv ) { 
	Mat dst, src = imread("/home/unoesc/Pictures/car.png", 1);
 
	// Converte para escala de cinza 
	cvtColor(src, src, CV_BGR2GRAY); 

	// Aplica equalizacao do histograma 
	equalizeHist(src, dst); 

	imshow("Original", src); 
	imshow("Equalizada", dst); 
	waitKey(0); 
}

