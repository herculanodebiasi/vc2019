/*
 * svm.cpp
 *
 *  Created on: Feb 14, 2017
 *      Author: debiasi
 */

// Arquivos de include do C++
#include <opencv2/core/mat.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

// Arquivos de include do OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/utility.hpp>

using namespace cv;
using namespace cv::ml;
using namespace std;

#include "utils/MultipleImageWindow.h"
MultipleImageWindow* miw;

Mat padrao_fundo, objeto;

Ptr<SVM> svm;
Scalar azul(255,0,0), verde(0,255,0), vermelho(0,0,255);

// Funções do OpenCV para parsing de argumentos em linha de comando
// Palavras-chave aceitas pelo parser
const char* chavesS =
{
   "{help h uso ? | | Imprime essa mensagem}"
   "{@image       | | Imagem a classificar}"
};

void plotaDadosTreinamento(Mat dadosTreinamento, Mat rotulos, float *erro=NULL) {
  float area_max, ar_max, area_min, ar_min;

  area_max = ar_max = 0;
  area_min = ar_min = 99999999;

  // Pega os valores mínimo e máximo de cada característica para normalizar a imagem plotada
  for(int i=0; i< dadosTreinamento.rows; i++){
    float area= dadosTreinamento.at<float>(i, 0);
    float ar= dadosTreinamento.at<float>(i, 1);

    if(area > area_max) area_max = area;
    if(ar > ar_max) 	ar_max = ar;
    if(area < area_min) area_min = area;
    if(ar < ar_min) 	ar_min = ar;
  }

  // Cria imagem a ser plotada
  Mat grafico = Mat::zeros(512, 512, CV_8UC3);

  // Plota cada uma das duas característcas em um gráfico 2D usando uma imagem
  // onde x é a área e y é a relação de aspecto
  for(int i=0; i< dadosTreinamento.rows; i++){
	// Define as coordenadas x e y de cada dado
    float area = dadosTreinamento.at<float>(i,0);
    float ar = dadosTreinamento.at<float>(i,1);
    int x = (int)(512.0f*((area-area_min)/(area_max-area_min)));
    int y = (int)(512.0f*((ar-ar_min)/(ar_max-ar_min)));

    // Pega rótulo
    int rotulo = rotulos.at<int>(i);

    // Define a cor dependendo do rótulo
    Scalar cor;
    if(rotulo==0)      cor = verde;    // Porca
    else if(rotulo==1) cor = azul;     // Arruela
    else if(rotulo==2) cor = vermelho; // Parafuso

    circle(grafico, Point(x, y), 3, cor, -1, 8);
  }

  if(erro != NULL){
    stringstream ss;
    ss << "Error: " << *erro << "\%";
    putText(grafico, ss.str().c_str(), Point(20,512-40), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(200,200,200), 1, LINE_AA);
  }

  miw->addImage("Grafico", grafico);
//  imshow("Grafico", grafico);
}


/**
* Extrai as características de todos os objetos em uma imagem
*
* @param Mat img - imagem de entrada
* @param vector<int> esquerda - saída das coordenadas da esquerda de cada objeto
* @param vector<int> topo - saída das coordenadas superiores de cada objeto
* @return vector< vector<float> >  - matriz de linhas das caraterísticas de cada objeto detectado
**/
vector<vector<float>> ExtraiCaracteristicas(Mat img, vector<int>* esquerda=NULL, vector<int>* topo=NULL) {
  vector<vector<float>> resultado;
  vector<vector<Point>> contornos;
  Mat entrada = img.clone();

  vector<Vec4i> hierarquia;
  findContours(entrada, contornos, hierarquia, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

  // Verifica o número de objetos detectados
  if(contornos.size() == 0) {
    return resultado;
  }
  
  RNG rng(0xFFFFFFFF);
  for(int i=0; i<contornos.size(); i++) {
    Mat mascara = Mat::zeros(img.rows, img.cols, CV_8UC1);
    drawContours(mascara, contornos, i, Scalar(1), FILLED, LINE_8, hierarquia, 1);
    Scalar area_s = sum(mascara);
    float area = area_s[0];

    if(area>500){ // Se a área é maior do que a mínima
      RotatedRect r = minAreaRect(contornos[i]);
      float comprimento = r.size.width;
      float altura = r.size.height;
      float proporcao = (comprimento<altura)?altura/comprimento:comprimento/altura;

      vector<float> elemento;
      elemento.push_back(area);
      elemento.push_back(proporcao);
      resultado.push_back(elemento);

      if(esquerda!=NULL) esquerda->push_back((int)r.center.x);
      if(topo!=NULL)     topo->push_back((int)r.center.y);

      objeto = mascara;
    }
  }

  return resultado;
}

/**
 * Remove th light and return new image without light
 * @param img Mat image to remove the light pattern
 * @param pattern Mat image with light pattern
 * @return a new image Mat without light
 */
Mat removeFundo(Mat img, Mat padrao) {
  Mat aux;

  // Necessário mudar imagem para float 32 bits para divisao
  Mat img32, padrao32;
  img.convertTo(img32, CV_32F);
  padrao.convertTo(padrao32, CV_32F);

  // Divide imagem pelo padrao
  aux = 1 - (img32 / padrao32);

  // Escalona cores para converter para 8 bits
  aux *= 255;

  // Converte para formato 8 bits
  aux.convertTo(aux, CV_8U);

  //equalizeHist( aux, aux );
  //aux = padrao - img;

  return aux;
}

/**
* Preprocess an input image to extract components and stats
* @params Mat input image to preprocess
* @return Mat binary image
*/
Mat preProcessaImagem(Mat entrada) {
  Mat resultado;

  // Remove ruído
  Mat img_sem_ruido, img_box_smooth;
  medianBlur(entrada, img_sem_ruido, 3);

  // Remove fundo
  Mat img_sem_fundo;
  img_sem_ruido.copyTo(img_sem_fundo);
  img_sem_fundo = removeFundo(img_sem_ruido, padrao_fundo);

  // Binariza imagem para segmentação
  threshold(img_sem_fundo, resultado, 30, 255, THRESH_BINARY);

  return resultado;
}

/**
* Read all images in a folder creating the train and test vectors
* @param folder string name
* @param label assigned to train and test data
* @param number of images used for test and evaluate algorithm error
* @param trainingData vector where store all features for training
* @param reponsesData vector where store all labels corresopinding for training data, in this case the label values
* @param testData vector where store all features for test, this vector as the num_for_test size
* @param testResponsesData vector where store all labels corresponiding for test, has the num_for_test size with label values
* @return true if can read the folder images, false in error case
**/
bool lePastaEExtraiCaracteristicas(string pasta, int rotulo, int num_para_teste,
  vector<float> &dadosTreinamento, vector<int> &dadosResposta,
  vector<float> &dadosTeste, vector<float> &dadosRespostasTestes) {
  VideoCapture imagens;

  if(imagens.open(pasta)==false){
    cout << "Erro ao abrir pasta de imagens" << endl;
    return false;
  }

  Mat quadro;
  int img_indice=0;
  while(imagens.read(quadro)){
	// Preprocessa frame
	Mat quadro_cinza;
	cvtColor(quadro, quadro_cinza, CV_BGR2GRAY);
    Mat pre = preProcessaImagem(quadro_cinza);

    // Extrai características
    vector< vector<float> > caracteristicas = ExtraiCaracteristicas(pre);
    for(int i=0; i< caracteristicas.size(); i++) {
      if(img_indice >= num_para_teste) {
        dadosTreinamento.push_back(caracteristicas[i][0]);
        dadosTreinamento.push_back(caracteristicas[i][1]);

        dadosResposta.push_back(rotulo);
      } else {
        dadosTeste.push_back(caracteristicas[i][0]);
        dadosTeste.push_back(caracteristicas[i][1]);

        dadosRespostasTestes.push_back((float)rotulo);
      }
    }
    img_indice++;
  }
  return true;
}

void treinaETesta() {
  vector<float> dadosTreinamento;
  vector<int> dadosResposta;
  vector<float> dadosTeste;
  vector<float> dadosRespostasTestes;

  int num_for_test= 20;

  // Recupera e processa as imagens de porcas
  lePastaEExtraiCaracteristicas("data/nut/tuerca_%04d.pgm", 0, num_for_test, dadosTreinamento, dadosResposta, dadosTeste, dadosRespostasTestes);

  // Recupera e processa as imagens de arruelas
  lePastaEExtraiCaracteristicas("data/ring/arandela_%04d.pgm", 1, num_for_test, dadosTreinamento, dadosResposta, dadosTeste, dadosRespostasTestes);

  // Recupera e processa as imagens de parafusos
  lePastaEExtraiCaracteristicas("data/screw/tornillo_%04d.pgm", 2, num_for_test, dadosTreinamento, dadosResposta, dadosTeste, dadosRespostasTestes);

  cout << "Numero de exemplos de treinamento: " << dadosResposta.size() << endl;
  cout << "Numero de exemplos de teste......: " << dadosRespostasTestes.size() << endl;

  // Une todos os dados
  Mat matrizDadosTreinamento(dadosTreinamento.size()/2, 2, CV_32FC1, &dadosTreinamento[0]);
  Mat respostas(dadosResposta.size(), 1, CV_32SC1, &dadosResposta[0]);

  Mat matrizDadosTeste(dadosTeste.size()/2, 2, CV_32FC1, &dadosTeste[0]);
  Mat respostasTestes(dadosRespostasTestes.size(), 1, CV_32FC1, &dadosRespostasTestes[0]);

  // Define os parâmetros da SVM
  svm = SVM::create();
  svm->setType(SVM::C_SVC);
  svm->setKernel(SVM::CHI2);
  svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));

  // Treina a SVM
  //Ptr<TrainData> td = TrainData::create(matrizDadosTreinamento, ROW_SAMPLE, respostas);
  svm->train(matrizDadosTreinamento, ROW_SAMPLE, respostas);

  if(dadosRespostasTestes.size()>0) {
    cout << "Avaliacao" << endl;
    cout << "=========" << endl;

    // Testa o modelo de ML
    Mat testaPredicao;
    svm->predict(matrizDadosTeste, testaPredicao);
    cout << "Predicao concluida!" << endl;

    // Cálculo do erro
    Mat matrizErros = (testaPredicao!=respostasTestes);
    float erro = 100.0f * countNonZero(matrizErros) / dadosRespostasTestes.size();
    cout << "Erro: " << erro << "\%" << endl;

    // Plota dados do treinamento com rótulo de erro
    plotaDadosTreinamento(matrizDadosTreinamento, respostas, &erro);
  } else {
    plotaDadosTreinamento(matrizDadosTreinamento, respostas);
  }
}

int main( int argc, const char** argv ) {
  CommandLineParser parser(argc, argv, chavesS);

  if (parser.has("help")) {
      parser.printMessage();
      return 0;
  }

  String img_file= parser.get<String>(0);
  String arq_padrao_luz= "data/pattern.pgm";

  if (!parser.check()) {
      parser.printErrors();
      return 0;
  }

  miw = new MultipleImageWindow("Janela", 2, 2, WINDOW_AUTOSIZE);

  // Carrega imagem
  Mat img = imread(img_file, 0);
  if(img.data==NULL) {
    cout << "Erro carregando imagem "<< img_file << endl;
    return 0;
  }

  Mat img_saida = img.clone();
  cvtColor(img_saida, img_saida, COLOR_GRAY2BGR);

  // Carrega imagem
  padrao_fundo = imread(arq_padrao_luz, 0);
  if(padrao_fundo.data==NULL){
	// Calcula padrao de fundo
    cout << "ERRO: Padrao de fundo nao carregado" << endl;
    return 0;
  }
  medianBlur(padrao_fundo, padrao_fundo, 3);

  // Pré-processa a imagem de entrada
  Mat pre = preProcessaImagem(img);

  // Extrai características
  vector<int> pos_topo, pos_esquerda;
  vector<vector<float>> caracteristicas = ExtraiCaracteristicas(pre, &pos_esquerda, &pos_topo);
  miw->addImage("Objeto", objeto*255);
  miw->render();

  // Extrai características
  treinaETesta();

  cout << "\nNumero de objetos detectados: " << caracteristicas.size() << endl;

  for(int i=0; i<caracteristicas.size(); i++) {
    cout << "\nArea: " << caracteristicas[i][0] << " Relacao de aspecto: " << caracteristicas[i][1] << endl;

    Mat matrizDadosTreinamento(1, 2, CV_32FC1, &caracteristicas[i][0]);

    float resultado = svm->predict(matrizDadosTreinamento);

    stringstream ss;
    Scalar cor;
    if(resultado==0) {
      cor = verde;    // Porca
      ss << "Porca";
    } else if(resultado==1) {
      cor = azul;     // Arruela
      ss << "Arruela";
    } else if(resultado==2) {
      cor = vermelho; // Parafuso
      ss << "Parafuso";
    }

    cout << "Objeto previsto: " << ss.str() << endl;

    putText(img_saida, ss.str(), Point2d(pos_esquerda[i], pos_topo[i]), FONT_HERSHEY_SIMPLEX, 0.4, cor);
  }

  // vector<int> results= evaluate(caracteristicas);
  // Mostra imagens
  miw->addImage("Imagem pre-processada", pre);
//  imshow("Imagem binaria", pre);
  miw->addImage("Resultado", img_saida);
//  imshow("Resultado", img_saida);
  miw->render();
  waitKey(0);

  return 0;
}
