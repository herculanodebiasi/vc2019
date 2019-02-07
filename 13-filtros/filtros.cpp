#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace cv;
using namespace std;

int main ( int, char** argv )
{
  /// Declare variables
  Mat src, dst;

  Mat kernel;
  Point anchor;
  double delta;
  int ddepth;
  int kernel_size;
  String window_name = "filter2D Demo";

  int c;

  src = imread( argv[1] ); // Load an image
  if( src.empty() ) {
    return -1;
  }

  namedWindow( window_name, WINDOW_AUTOSIZE ); // Create window

  /// Initialize arguments for the filter
  anchor = Point( -1, -1 );
  delta = 0;
  ddepth = -1;

  /// Loop - Will filter the image with different kernel sizes each 0.5 seconds
  int ind = 0;
  for(;;) {
    c = waitKey(500);
    /// Press 'ESC' to exit the program
    if( (char)c == 27 ) {
      break;
    }

    /// Update kernel size for a normalized box filter
    kernel_size = 3 + 2*( ind%5 );
    kernel = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);

    cout << kernel << endl;

    /// Apply filter
    filter2D(src, dst, ddepth , kernel, anchor, delta, BORDER_DEFAULT );
    imshow( window_name, dst );
    ind++;
  }

  return 0;
}
