#include <opencv2/highgui/highgui.hpp>
#include <string.h>

using namespace cv;

void zeraCanais(Mat canais[], int c1, int c2, int linhas, int colunas) {
    canais[c1] = Mat::zeros(linhas, colunas, CV_8UC1);
    canais[c2] = Mat::zeros(linhas, colunas, CV_8UC1);
}

void divideCanais(Mat im, int canal) {
    int linhas = im.rows;
    int colunas = im.cols;
    String nome_canal;

    Mat im_clone=im.clone();
    Mat canais[3];
    split(im_clone, canais);

    if(canal == 0) {
      nome_canal = "Canal B";
      zeraCanais(canais, 1, 2, linhas, colunas);
    } else if(canal == 1) {
      nome_canal = "Canal G";
      zeraCanais(canais, 0, 2, linhas, colunas);
    } else {
      nome_canal = "Canal R";
      zeraCanais(canais, 0, 1, linhas, colunas);
    }

    imshow(nome_canal + " (intensidade)", canais[canal]);
    merge(canais, 3, im_clone);
    imshow(nome_canal, im_clone);
}

int main()
{
    Mat im = imread("/home/debiasi/Pictures/circulos.png");
    imshow("Imagem", im);

    for(int i=0; i<3; i++) {
      divideCanais(im, i);
    }

    waitKey(0);
}
