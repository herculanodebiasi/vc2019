#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// quantize the image to numBits
Mat quantizeImage(const cv::Mat& inImage, int numBits)
{
    Mat retImage = inImage.clone();
    int planes = retImage.channels();
    uchar maskBit = 0xFF;

    // keep numBits as 1 and (8 - numBits) would be all 0 towards the right
    maskBit = maskBit << (8 - numBits);

    for(int j = 0; j < retImage.rows; j++) {
        for(int i = 0; i < retImage.cols; i++)
        {
            if(planes == 3) {
                Vec3b valVec = retImage.at<Vec3b>(j, i);
                valVec[0] = valVec[0] & maskBit;
                valVec[1] = valVec[1] & maskBit;
                valVec[2] = valVec[2] & maskBit;
                retImage.at<Vec3b>(j, i) = valVec;
	    } else {
                uchar valVec = retImage.at<uchar>(j, i);
                retImage.at<uchar>(j, i) = valVec & maskBit;
	    }
        }
   }

   return retImage;
}


int main ()
{
    Mat inImage, glImage;
    inImage = cv::imread("landscape.jpg", IMREAD_GRAYSCALE);

    char buffer[30];
    for(int i = 1; i <= 8; i++)
    {
        Mat quantizedImage = quantizeImage(inImage, i);
        sprintf(buffer, "%d Bit Image", i);
        imshow(buffer, quantizedImage);

        sprintf(buffer, "%d Bit Image.png", i);
        imwrite(buffer, quantizedImage);
    }

    waitKey(0);
    return 0;
}

