#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main() {
	Mat res, im = cv::imread("/home/debiasi/Pictures/lena.png");

	blur(im, res, Size(11, 11));
	imshow("Blur", res);

	GaussianBlur(im, res, Size(11, 11), 1.5);
	imshow("Gaussian Blur", res);

	medianBlur(im, res, 11);
	imshow("Median Blur", res);

	waitKey(0);
}

