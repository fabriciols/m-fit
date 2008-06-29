#include "cv.h"
#include "highgui.h"

#include <stdexcept>

#include "../include/Frame.h"
#include "../include/Log.h"

#define HIST_WIDTH  256
#define HIST_HEIGHT 256

/************************************************************************
* Construtor para Frame que recebe os valores necess�rios para plotar
* um histograma. Cria um objeto Frame com o desenho do histograma.
*************************************************************************
* param (E): double matrix[] - Valores do Histograma
* param (E): int len_i       - Quantidade de valores em 'matrix'
* param (E): float max_f     - Valor m�ximo do histograma
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Frame::Frame(double matrix[], int len_i, float max_f)
{

	int i;
	int normalized;

	IplImage* imgHistogram = 0;

	imgHistogram = cvCreateImage(cvSize(len_i*2, HIST_HEIGHT), 8, 1);

	// Desenha a area de trabalho do histograma
	cvRectangle(imgHistogram, cvPoint(0,0), cvPoint(HIST_WIDTH*2,HIST_HEIGHT), CV_RGB(255,255,255), -1);

	// Desenhar as linhas do histograma
	for (i=0 ; i < len_i ; i++)
	{
		normalized = cvRound((matrix[i]*HIST_HEIGHT)/max_f);

		Log::writeLog("%s :: Plot Value[%3d] = [%4.0lf] Normalized = [%3d]", __FUNCTION__, i, matrix[i], normalized);

		// Printa a linha do Histograma
		cvLine(imgHistogram, cvPoint((i*2)-1,HIST_HEIGHT), cvPoint((i*2)-1,HIST_HEIGHT-normalized), CV_RGB(0, 0, 0));

		// Print uma linha em branco, pro histograma nao ficar todo grudado
		cvLine(imgHistogram, cvPoint(i*2,HIST_HEIGHT), cvPoint(i*2,HIST_HEIGHT-normalized), CV_RGB(0, 255, 255));

	}

	this->data = imgHistogram;

	// Aponta para os valores do IplImage
	this->width =  &this->data->width;
	this->height = &this->data->height;

}


/************************************************************************
* Construtor para Frame que recebe um caminho para um arquivo no disco.
* Tenta abrir o arquivo de imagem e constroi um objeto Frame a partir dele
*************************************************************************
* param (E): char* filename_cy - String com o caminho para a imagem.
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Frame::Frame(char *filename_cy)
{
	IplImage* img;
	img = cvLoadImage(filename_cy, 1);

	if (img == NULL)
	{
		throw "Cant open file";
	}

	// Guardamos a inst�ncia do IplImage na estrutura
	// do frame.
	this->data = img;

	// Aponta para os valores do IplImage
	this->width =  &this->data->width;
	this->height = &this->data->height;

}

/************************************************************************
* Construtor para Frame que recebe uma imagem j� carregada.
* Cria um Frame com base nos dados da imagem passada.
*************************************************************************
* param (E): IplImage* img_src - Imagem a ser usada como base para a
* cria��o de um Frame
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Frame::Frame(IplImage *img_src)
{
	// Guardamos a inst�ncia do IplImage na estrutura
	// do frame.
	this->data = img_src;

	// Aponta para os valores do IplImage
	this->width =  &this->data->width;
	this->height = &this->data->height;
}

/************************************************************************
* Construtor para Frame que recebe um frame j� instanciado.
* Clona o frame.
*************************************************************************
* param (E): Frame *frame - Frame a ser clonado.
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Frame::Frame(Frame *frame)
{
	// Faz a copia do objeto
	IplImage *img = new IplImage();

	memcpy(img, frame->data, sizeof(IplImage));

	this->data = img;

	this->width =  &this->data->width;
	this->height = &this->data->height;

}

/************************************************************************
* Fun��o que converte o frame para escala de cinza.
*************************************************************************
* param (E): Nenhum.
*************************************************************************
* return : Frame* - Retorna um ponteiro para um novo frame.
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Frame* Frame::convert2Gray()
{
	IplImage *imgGray;

	// Cria a img com as mesmas dimens�es da inst�ncia atual
	// por�m com 8 bits e 1 canal apenas
	imgGray = cvCreateImage(cvGetSize(this->data),8,1);

	// Ordena os bytes da imagem, sen�o ela ficar� de ponta cabe�a
	imgGray->origin = this->data->origin;

	// Converte pra cinza;
	cvCvtColor(this->data, imgGray, CV_RGB2GRAY);

	return (new Frame(imgGray));
}

/************************************************************************
* Fun��o que retorna a largura do frame.
*************************************************************************
* param (E): Nenhum.
*************************************************************************
* return : int - Largura do frame.
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
int Frame::getWidth()
{
	return(*this->width);
}

/************************************************************************
* Fun��o que retorna a altura do frame.
*************************************************************************
* param (E): Nenhum.
*************************************************************************
* return : int - Altura do frame.
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
int Frame::getHeight()
{
	return(*this->height);
}
