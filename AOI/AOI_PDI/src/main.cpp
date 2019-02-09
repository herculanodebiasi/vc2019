/*
 * main.cpp
 *
 *  Created on: Feb 11, 2017
 *      Author: debiasi
 */
// Arquivos de include do C++
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

// Arquivos de include do OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/utility.hpp>

#include "utils/MultipleImageWindow.h"
MultipleImageWindow* miw;

// Namespaces
using namespace std;
using namespace cv;

// Funções do OpenCV para parsing de argumentos em linha de comando
// Palavras-chave aceitas pelo parser
const char* chavesM =
	{
	"{help h uso ?  |   | imprime esta mensagem}"
	"{@image        |   | Imagem a processar}"
	"{@lightPattern |   | Imagem com padrao de luz a aplicar na imagem de entrada}"
	"{lightMethod   | 1 | Metodo para remover a luz de fundo, 0 diferenca, 1 divisao}"
	"{segMethod     | 1 | Metodo para segmentar: 1 componentes conexas, 2 componentes conexas com estatisticas, 3 encontrar contornos}"
	};

static Scalar corAleatoria(RNG& rng) {
	int icor = (unsigned) rng;

	return Scalar(icor&255, (icor>>8)&255, (icor>>16)&255);
}

Mat thresholding(Mat img_sem_luz, int metodo_luz) {
	// Segmentação através de binarização
	Mat img_thr;

	if(metodo_luz != 2) {
		threshold(img_sem_luz, img_thr, 30, 255, THRESH_BINARY);
	} else {
		threshold(img_sem_luz, img_thr, 140, 255, THRESH_BINARY_INV);
	}

	return(img_thr);
}

// Calculate image pattern from an input image
Mat calculaPadraoLuz(Mat img)
{
	Mat padrao;
	// Basic and effective way to calculate the light pattern from one image
	blur(img, padrao, Size(img.cols/3, img.cols/3));

	return padrao;
}

Mat removeLuz(Mat img, Mat padrao, int metodo) {
	Mat aux;

	// Metodo 1: normalizacao
	if(metodo == 1) {
		// Necessario mudar imagem para float 32 bits para divisao
		Mat img32, padrao32;
		img.convertTo(img32, CV_32F);
		padrao.convertTo(padrao32, CV_32F);

		// Divide imagem pelo padrao
		aux = 1 - (img32 / padrao32);

		// Escalona cores para converter para 8 bits
		aux *= 255;

		// Converte para formato 8 bits
		aux.convertTo(aux, CV_8U);
	} else{
		aux = padrao - img;
	}

	return aux;
}

Mat removeFundo(String arq_padrao_fundo, Mat img, int metodo_luz) {
	// Carrega imagem
	Mat padrao_fundo = imread(arq_padrao_fundo, 0);

	if(padrao_fundo.data == NULL){
		// Calcula padrao de luz
		padrao_fundo = calculaPadraoLuz(img);
	}
	medianBlur(padrao_fundo, padrao_fundo, 7);

	// Remove fundo
	Mat img_sem_fundo;
	img.copyTo(img_sem_fundo);
	if(metodo_luz != 2) {
		img_sem_fundo = removeLuz(img, padrao_fundo, metodo_luz);
	}

	miw->addImage("Fundo", padrao_fundo);
//	imshow("Fundo", padrao_fundo);

	return img_sem_fundo;
}

Mat removeRuido(Mat imagem) {
	// Remove ruido
  	Mat img_sem_ruido;

	medianBlur(imagem, img_sem_ruido, 7);

	return img_sem_ruido;
}

void verificaNumObjDetectados(int num_objetos) {
	// Verifica o numero de objetos detectados
	if(num_objetos < 2 ){
		cout << "Nenhum objeto detectado" << endl;
		exit(0);
	} else {
		cout << "Numero de objetos detectados: " << (num_objetos - 1) << endl;
	}
}

Mat ComponentesConexas(Mat img) {
	// Usa componentes conexas para segmentar partes da imagem
	Mat rotulos;
	int num_objetos = connectedComponents(img, rotulos);
	verificaNumObjDetectados(num_objetos);

	// Cria imagem de saída colorindo objetos
	Mat resultado= Mat::zeros(img.rows,img.cols, CV_8UC3);
	RNG rng(0xFFFFFFFF);

	for(int i=1; i<num_objetos; i++) {
		Mat mascara = (rotulos==i);
		resultado.setTo(corAleatoria(rng), mascara);
	}

	return resultado;
}

Mat ComponentesConexasComEstatisticas(Mat img) {
	// Usa componentes conexas com estatisticas
	Mat rotulos, estatisticas, centroides;
	int num_objetos = connectedComponentsWithStats(img, rotulos, estatisticas, centroides);
	verificaNumObjDetectados(num_objetos);

	// Cria imagem de saída colorindo objetos e mostra área
	Mat resultado = Mat::zeros(img.rows, img.cols, CV_8UC3);
	RNG rng(0xFFFFFFFF);

	for(int i=1; i<num_objetos; i++){
		cout << "Objeto "<< i << " posicao: " << centroides.at<Point2d>(i) << " e area " << estatisticas.at<int>(i, CC_STAT_AREA) << endl;

		Mat mascara = (rotulos==i);
		resultado.setTo(corAleatoria(rng), mascara);

		stringstream ss;
		ss << "area: " << estatisticas.at<int>(i, CC_STAT_AREA);

		putText(resultado, ss.str(), centroides.at<Point2d>(i), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255,255,255));
	}

	return resultado;
}

Mat EncontraContornos(Mat img) {
	vector<vector<Point> > contornos;
	findContours(img, contornos, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	Mat resultado = Mat::zeros(img.rows,img.cols, CV_8UC3);

	// Verifica o número de objetos detectados
	if(contornos.size() == 0) {
		cout << "Nenhum objeto detectado" << endl;
		exit(0);
	} else {
		cout << "Numero de objetos detectados: " << contornos.size() << endl;
	}

	RNG rng(0xFFFFFFFF);
	for(int i=0; i<contornos.size(); i++) {
		drawContours(resultado, contornos, i, corAleatoria(rng));
	}

	return resultado;
}

void mostraResultados(Mat entrada, Mat sem_ruido, Mat sem_fundo, Mat thr, Mat componentes) {
	// Mostra imagens
	//imshow("Entrada", entrada);
	miw->addImage("Entrada", entrada);
	//imshow("Sem ruido", sem_ruido);
	miw->addImage("Entrada sem ruido", sem_ruido);
	//miw->addImage("Input without noise with box smooth", img_box_smooth);

	//imshow("Sem Fundo", sem_fundo);
	miw->addImage("Sem fundo", sem_fundo);

	//imshow("Threshold", thr);
	miw->addImage("Threshold (binarizacao)", thr);

	//imshow("Resultado", componentes);
	miw->addImage("Resultado", componentes);
	miw->render();
}

int main(int argc, const char** argv) {
	CommandLineParser parser(argc, argv, chavesM);

	if (parser.has("help")) {
		parser.printMessage();
		return 0;
	}

	String img_arquivo = parser.get<String>(0);
	String arq_padrao_luz = parser.get<String>(1);
	int metodo_luz = parser.get<int>("lightMethod");
	int metodo_seg = parser.get<int>("segMethod");

	if(!parser.check()) {
		parser.printErrors();
		return 0;
	}

	// Carrega imagem
	Mat img = imread(img_arquivo, 0);
	if(img.data == NULL){
		cout << "Erro ao carregar imagem "<< img_arquivo << endl;
		return 0;
	}

	// Cria janela para múltiplas imagens
	miw = new MultipleImageWindow("Janela", 3, 2, WINDOW_AUTOSIZE);

	// Remove ruido
	Mat img_sem_ruido = removeRuido(img);

	// Remove fundo
	Mat img_sem_fundo = removeFundo(arq_padrao_luz, img_sem_ruido, metodo_luz);

	// Thresholding
	Mat img_thr = thresholding(img_sem_fundo, metodo_luz);

	// Componentes conexas
	Mat img_componentes;
	switch(metodo_seg) {
	  case 1:
		img_componentes = ComponentesConexas(img_thr);
		break;
	  case 2:
		img_componentes = ComponentesConexasComEstatisticas(img_thr);
		break;
	  case 3:
		img_componentes = EncontraContornos(img_thr);
		break;
	}

	mostraResultados(img, img_sem_ruido, img_sem_fundo, img_thr, img_componentes);

	waitKey(0);
}
