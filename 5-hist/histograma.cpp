#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int, char**)
{
    Mat gray=imread("/home/debiasi/Pictures/lena.png",0);
    imshow("Gray", gray);

    // Inicializa os parametros
    int histSize = 256;    // numero de niveis
    float range[] = {0, 255};
    const float *ranges[] = {range};

    // Calcula histograma
    MatND hist;
    calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, ranges, true, false);
    // imagens, num_imagens, canais, mascara, saida_histograma, dimensoes, vetor com tamanho das dimensoes do histograma, faixa, uniforme, acumulado

    // Imprime o histograma calculado
    cout << "Histograma: " << endl;
    for( int h = 0; h < histSize; h++ ) {
       float binVal = hist.at<float>(h);
       cout << " " << binVal;
    }

    // Plota o histograma
    int hist_w = 512;
    int hist_h = 200;
    int bin_w = cvRound((double) hist_w/histSize);

    Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));

    // Normaliza os valores
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    // Imprime o histograma normalizado
    cout << "\n\n\nHistograma: " << endl;
    for( int h = 0; h < histSize; h++ ) {
       float binVal = hist.at<float>(h);
       cout << " " << binVal;
    }

    // Plota o histograma
    for( int i = 1; i < histSize; i++ ) {
      line(histImage,
	   Point(bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1))),
           Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))),
           Scalar(255, 0, 0), 2, 8, 0);
    }

    imshow("Result", histImage);

    waitKey(0);
    return 0;
}
