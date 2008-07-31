#include "cv.h"
#include "highgui.h"

#include <stdexcept>
#include <math.h>

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Log.h"

#define HIST_WIDTH  256
#define HIST_HEIGHT 256

/************************************************************************
* Auxiliar que atualiza as variaveis internas de um objeto Frame 
* deve ser usada toda vez
*************************************************************************
* param (E): Nenhum
*************************************************************************
* Histórico:
* 29/07/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
void Frame::setImage(IplImage* imgNew)
{
	// Nao esta funcionando
	// precisa dar uma entendida o por que
	if (this->data != NULL)
	{
		Log::writeLog("%s :: old: this->data[%x]", __FUNCTION__, this->data);

		// Desaloca
		cvReleaseImage(&this->data);
	}

	// Aponta para a img nova
	this->data = imgNew;

	// Aponta para os valores do IplImage
	this->width =  &this->data->width;
	this->height = &this->data->height;

	Log::writeLog("%s :: new: this->data[%x]", __FUNCTION__, imgNew);
}

/************************************************************************
* Inicializa todos os atributos com nulo
* Esta função deve ser usada em TODOS os construtores
*************************************************************************
* param (E): Nenhum
*************************************************************************
* Histórico:
* 29/07/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
void Frame::initAttr()
{
	this->data = NULL;
	this->width =  NULL;
	this->height = NULL;
}

/************************************************************************
* Construtor para Frame que recebe os valores necessários para plotar
* um histograma. Cria um objeto Frame com o desenho do histograma.
*************************************************************************
* param (E): double matrix[] - Valores do Histograma
* param (E): int len_i       - Quantidade de valores em 'matrix'
* param (E): float max_f     - Valor máximo do histograma
*************************************************************************
* Histórico:
* 27/06/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
Frame::Frame(double *matrix, int len_i, float max_f)
{

	int i;
	int normalized;

	IplImage* imgHistogram = 0;

	//imgHistogram = cvCreateImage(cvSize(len_i*2, HIST_HEIGHT), 8, 1);
	imgHistogram = cvCreateImage(cvSize(len_i, HIST_HEIGHT), 8, 1);

	// Desenha a area de trabalho do histograma
	//cvRectangle(imgHistogram, cvPoint(0,0), cvPoint(len_i*2,HIST_HEIGHT), CV_RGB(255,255,255), -1);
	cvRectangle(imgHistogram, cvPoint(0,0), cvPoint(len_i,HIST_HEIGHT), CV_RGB(255,255,255), -1);

	// Desenhar as linhas do histograma
	for (i=0 ; i < len_i ; i++)
	{
		normalized = cvRound((matrix[i]*HIST_HEIGHT)/max_f);

		Log::writeLog("%s :: Plot Value[%3d] = [%4.0lf] Normalized = [%3d]", __FUNCTION__, i, matrix[i], normalized);

		// Printa a linha do Histograma
		//cvLine(imgHistogram, cvPoint((i*2)-1,HIST_HEIGHT), cvPoint((i*2)-1,HIST_HEIGHT-normalized), CV_RGB(0, 0, 0));
		cvLine(imgHistogram, cvPoint((i)-1,HIST_HEIGHT), cvPoint((i)-1,HIST_HEIGHT-normalized), CV_RGB(0, 0, 0));

		// Print uma linha em branco, pro histograma nao ficar todo grudado
		//cvLine(imgHistogram, cvPoint(i*2,HIST_HEIGHT), cvPoint(i*2,HIST_HEIGHT-normalized), CV_RGB(0, 255, 255));

	}

	initAttr();
	setImage(imgHistogram);

}


/************************************************************************
* Construtor para Frame que recebe um caminho para um arquivo no disco.
* Tenta abrir o arquivo de imagem e constroi um objeto Frame a partir dele
*************************************************************************
* param (E): char* filename_cy - String com o caminho para a imagem.
*************************************************************************
* Histórico:
* 27/06/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
Frame::Frame(char *filename_cy)
{
	IplImage* img;

	img = cvLoadImage(filename_cy, 1);

	if (img == NULL)
	{
		throw "Cant open file";
	}

	initAttr();
	setImage(img);
}

/************************************************************************
* Construtor para Frame que recebe uma imagem já carregada
* faz uma copia dessa imagem, e deixa o endereco no frame.
* Cria um Frame com base nos dados da imagem passada.
*************************************************************************
* param (E): IplImage* img_src - Imagem a ser usada como base para a
* criação de um Frame
*************************************************************************
* Histórico:
* 27/06/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
Frame::Frame(IplImage *img_src)
{
	IplImage *imgNew;

	Log::writeLog("%s :: Constructor param: IplImage[%x]", __FUNCTION__, img_src);
	initAttr();

	imgNew = cvCreateImage(cvSize(img_src->width, img_src->height), img_src->depth, img_src->nChannels);

	cvCopy(img_src, imgNew);

	setImage(imgNew);
}

/************************************************************************
* Construtor para Frame que recebe um frame já instanciado.
* Clona o frame.
*************************************************************************
* param (E): Frame *frame - Frame a ser clonado.
*************************************************************************
* Histórico:
* 27/06/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
Frame::Frame(Frame *frame)
{
	IplImage *img;

	// Faz a copia do objeto
	img = cvCreateImage(cvSize(frame->getWidth(), frame->getHeight()), frame->data->depth, frame->data->nChannels);

	cvCopy(frame->data, img);

	initAttr();
	setImage(img);

}

/************************************************************************
* Função que retorna a largura do frame.
*************************************************************************
* param (E): Nenhum.
*************************************************************************
* return : int - Largura do frame.
*************************************************************************
* Histórico:
* 27/06/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
int Frame::getWidth()
{
	return(*this->width);
}

/************************************************************************
* Função que retorna a altura do frame.
*************************************************************************
* param (E): Nenhum.
*************************************************************************
* return : int - Altura do frame. 
*************************************************************************
* Histórico:
* 27/06/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
int Frame::getHeight()
{
	return(*this->height);
}

/************************************************************************
* FUNÇÃO QUE PEGA A DIAGONAL DE UM FRAME. 
*************************************************************************
* Esta função utiliza as fórmulas de trigonometria para encontrar a equa
* ção da reta a partir disto obter todos os pontos da diagonal do frame. 
*************************************************************************
* param (E): Nenhum
*************************************************************************
* return : Frame* -> Frame com a diagonal
*************************************************************************
* Histórico:
* 29/07/08 - Fabrício Lopes de Souza
* 'N' alterações para fazer funcionar, precisa-se validar com perfeição
* para verificar se está correto.
* 15/07/08 - Thiago Mizutani
* Adequação do processo de montagem da diagonal do frame. Utilizando as
* funções de getPixel e set Pixel.
* 06/07/08 - Thiago Mizutani
* Ajustes para extracao dos pontos da diagonal principal do frame.
* 04/07/08 - Thiago Mizutani
* Criação.
************************************************************************/
Frame * Frame::getDiagonal()
{
	int x = 0; // x da equacao da reta
	int y = 0; // y da equacao da reta

	Frame* frameDiagonal;

	int luminance = 0; //Valor de luminancia do pixel retirado da diagonal.
	
	double a = 0; // Coeficiente angular da equacao

	Log::writeLog("%s :: diagonal img width[%d] height[%d]", __FUNCTION__, 1, this->getWidth());

	IplImage* imgDiagonal = cvCreateImage(cvSize(1, this->getWidth()), 8, 1);

	frameDiagonal = new Frame(imgDiagonal);

	/** Calculo o coeficiente angular da reta ('a' da equacao).
	 * this->getHeight = y - yo
	 * this->getWidth  = x - xo
	 * y - yo = m*(x - xo)
	 **/
	a = (float)(this->getHeight()/(float)this->getWidth());

	// Pego a diagonal do this.
	for (x=0; x < this->getWidth() ; x++)
	{
		y = cvRound(a * x);

		// Monto o this da diagonal (já transformado em coluna).
		frameDiagonal->setPixel(0, x, this->getPixel(x, y));
	}	

	return(frameDiagonal);

}

/************************************************************************
 * Função que define a luminancia de um pixel
 *************************************************************************
 * param (E): int x   -> coordenada x do pixel desejado
 * param (E): int y   -> coordenada y do pixel desejado
 * param (E): int lum -> valor de luminancia a ser definido no pixel (x,y)
 *************************************************************************
 * Histórico:
 * 14/07/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
void Frame::setPixel(int x, int y, int lum)
{
	CvScalar s;

	s.val[0] = lum;

	cvSet2D(this->data, y, x, s);
}

/************************************************************************
 * Função que retorna o valor de luminancia de um determinado pixel (x,y)
 *************************************************************************
 * param (E): int x   -> coordenada x do pixel desejado
 * param (E): int y   -> coordenada y do pixel desejado
 *************************************************************************
 * Histórico:
 * 14/07/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
int Frame::getPixel(int x, int y)
{
	//return ((uchar*)(this->data->imageData + this->data->widthStep*y))[x];
	CvScalar s;

	s = cvGet2D(this->data, y, x);

	return cvRound(s.val[0]);
}

/************************************************************************
* Destructor da classe frame. Libera a memoria da imagem
*************************************************************************
* param (E): Nenhum
*************************************************************************
* Histórico:
* 28/07/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
Frame::~Frame()
{
	// Se o ponteiro para a imagem nao for nulo
	if (this->data)
	{
		// Libera a memoria alocada para ele
		cvReleaseImage(&this->data);
	}
}

/************************************************************************
* Sobrecarga do operador '+=' ele ira realizar a concatenacao dos frames
* Obs.: Atencao para os parametros, pois o soma tem que ser a dois objetos 
* absolutos, e nao dois ponteiros
*************************************************************************
* param (E): Frame frame -> Frame a ser concatenado
*************************************************************************
* Histórico:
* 28/07/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
Frame & Frame::operator+=(Frame &frame)
{

	IplImage *imgDst;

	Log::writeLog("%s :: this->data[%x]", __FUNCTION__, this->data);

	// As imagens tem que ter obrigatoriamente a mesma altura
	if (this->getHeight() != frame.getHeight())
		throw;

	// Crio uma imagem com a largura igual a soma das larguras
	imgDst = cvCreateImage(cvSize(this->getWidth() + frame.getWidth(), this->getHeight()), this->data->depth, this->data->nChannels);

	Log::writeLog("%s :: img_dst width[%d] height[%d]", __FUNCTION__, imgDst->width, imgDst->height);

	Log::writeLog("%s :: this x[%d] y[%d] width[%d] height[%d]", __FUNCTION__, 0, 0, this->getWidth(), this->getHeight());

	// Na imagem destino, seto a area de interesse o espaco que a primeira imagem ira ocupar
	cvSetImageROI(imgDst,
			cvRect
			(
			 0,
			 0,
			 this->getWidth(),
			 this->getHeight())
			);

	// então copio esta imagem para esta área
	cvCopy(this->data,imgDst);

	Log::writeLog("%s :: frame x[%d] y[%d] width[%d] height[%d]", __FUNCTION__, this->getWidth(), 0, frame.getWidth(), frame.getHeight());

	// Agora, seto a area de interesse o espaco que a segunda imagem irá ocupar
	cvSetImageROI(imgDst,
			cvRect
			(
			 this->getWidth(),
			 0,
			 frame.getWidth(),
			 frame.getHeight())
			);

	// Copia a segunda parte do frame
	cvCopy(frame.data,imgDst);

	// Libera a memoria alocada pela imagem anterior
	cvReleaseImage(&this->data);

	// Removo as áreas de interesse da imagem
	cvResetImageROI(imgDst);

	// Seta pra imagem nova
	setImage(imgDst);

	Log::writeLog("%s :: this->data[%x]", __FUNCTION__, this->data);

}

/************************************************************************
* Função que chama o construtor do histograma e retorna o histograma
* do frame. 
*************************************************************************
* param (E): IplImage* frame => Frame do qual será gerado o histograma
* param (S): Histogram* &histogram => Histograma do frame de entrada
*************************************************************************
* Histórico:
* 29/07/08 - Thiago Mizutani
* Criação.
************************************************************************/

Histogram* Frame::createHistogram()
{
	return new Histogram(this->data);
}

void Frame::write(char *filename_cy)
{
	if(!cvSaveImage(filename_cy, this->data))
	{
		printf("Could not save: %s\n",filename_cy);
	}
}
