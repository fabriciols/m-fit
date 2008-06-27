#include "cv.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Log.h"

#define HIST_WIDTH  256
#define HIST_HEIGHT 256

/************************************************************************
* Construtor para Histogram que recebe uma imagem do tipo OPENCV
* e com isso gera o histograma, preenchedo os atributos necess�rios.
*************************************************************************
* param (E): IplImage* img - Imagem j� alocada.
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Histogram::Histogram(IplImage* img)
{
	// Tamanho de um histograma 1D
	int bins = HIST_WIDTH;
	int hsize[] = {bins};

	//IplImage* imgHistogram = 0;

	CvHistogram* hist = 0;


	float value;
	int normalized;

	// Ranges, escala de cinza: 0-255
	float xranges[] = {0, 256};
	float* ranges[] = {xranges};

	int i;

	float max = 0, min = 0;
	int idx_max = 0, idx_min = 0;

	double soma = 0;

	IplImage* planes[] = {img};

	// Cria a estrutura m�ninima para o histograma
	hist = cvCreateHist(1, hsize, CV_HIST_ARRAY, ranges, 1);

	// Faz o calculo do histograma
	cvCalcHist(planes, hist, 0, NULL);

	// Pega os valores de escala de cinza mais e menos presentes na img
	cvGetMinMaxHistValue(hist, &min, &max, &idx_min, &idx_max);


	// Monta a matrix com os valores do histograma
	for (i=0 ; i < bins ; i++)
	{
		// Pega o quantas vezes a escala de cinza 'i' aparece na imagem
		this->data[i] = cvQueryHistValue_1D(hist, i);

		// Variavel criada apenas para ter um controle
		// pois a soma de todos os pixels tem que bater
		// com a quantidade de pixels que tem na imagem (obvio?)
		soma += this->data[i];

	}

	this->max = max;
	this->min = min;
	this->idx_max = idx_max;
	this->idx_min = idx_min;

	Log::writeLog("%s :: min[%d]: [%.0f], max[%d]: [%.0f]", __FUNCTION__, this->idx_min, this->min, this->idx_max, this->max);
	Log::writeLog("%s :: soma_pixels[%.0lf] == width*height[%.0ld]", __FUNCTION__, soma, img->width * img->height);

}

/************************************************************************
* Fun��o que retorna os dados do histograma.
*************************************************************************
* param (E): Nenhum.
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
double* Histogram::getMatrix()
{
	return this->data;
}

/************************************************************************
* Fun��o que retorna a quantidade de pixels de ocorr�ncia da escala 
* de cinza que tem maior presen�a na imagem
*************************************************************************
* param (E): Nenhum.
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
float Histogram::getMax()
{
	return this->max;
}

/************************************************************************
* Fun��o que retorna a quantidade de pixels de ocorr�ncia da escala 
* de cinza que tem menor presen�a na imagem
*************************************************************************
* param (E): Nenhum.
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
float Histogram::getMin()
{
	return this->min;
}

/************************************************************************
* Fun��o que retorna o valor da escala de cinza que tem maior presen�a
* na imagem
*************************************************************************
* param (E): Nenhum.
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
int Histogram::getMaxIdx()
{
	return this->idx_max;
}

/************************************************************************
* Fun��o que retorna o valor da escala de cinza que tem menor presen�a
* na imagem
*************************************************************************
* param (E): Nenhum.
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
int Histogram::getMinIdx()
{
	return this->idx_min;
}
