#include "cv.h"
#include "highgui.h"

#include <stdexcept>
#include <math.h>

#include "../include/Frame.h"
#include "../include/Log.h"

#define HIST_WIDTH  256
#define HIST_HEIGHT 256

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

	// Guardamos a instância do IplImage na estrutura
	// do frame.
	this->data = img;

	// Aponta para os valores do IplImage
	this->width =  &this->data->width;
	this->height = &this->data->height;

}

/************************************************************************
* Construtor para Frame que recebe uma imagem já carregada.
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

	Log::writeLog("%s :: Constructor param: IplImage[%x]", __FUNCTION__, img_src);

	// Guardamos a instância do IplImage na estrutura
	// do frame.
	this->data = img_src;

	// Aponta para os valores do IplImage
	this->width =  &this->data->width;
	this->height = &this->data->height;
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
	// Faz a copia do objeto
	IplImage *img = new IplImage();

	memcpy(img, frame->data, sizeof(IplImage));

	this->data = img;

	this->width =  &this->data->width;
	this->height = &this->data->height;

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
* return : int - Altura do frame. lum = ((uchar*)(fgray->imageData + fgray->widthStep*y))[x]; 
     ((uchar*)(imvr->imageData + imvr->widthStep*x))[f] = lum;
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
* Função que pega a diagonal de um frame. 
*************************************************************************
* param (E): Frame* frame => frame do qual sera retirada a diagonal.
* 				 int column => coordenada x do pixel que será copiado.
*************************************************************************
* return : diagonal do frame.
*************************************************************************
* Histórico:
* 06/07/08 - Thiago Mizutani
* Ajustes para extracao dos pontos da diagonal principal do frame.
* 04/07/08 - Thiago Mizutani
* Criação.
************************************************************************/
int Frame::getDiagonal(Frame* frame, int column)
{
	/** y = a*x+b (equacao da reta) 
	 * onde a = cateto oposto/cateto adjacente
	 * ou seja, y - yo = m*(x - xo), sendo que 
	 * y-yo = altura do frame e x - xo = largura do frame.
   **/
	
	// x = x final, xo = x final, o mesmo vale para o y.
	int x = 0;
	int y = 0;
	int i = 0;
	int luminance = 0; //Valor de luminancia do pixel retirado da diagonal.

	float a = 0; // Coeficiente angular da equacao
	int b = 0; // Ponto onde a reta cruza o eixo Y (x = 0).

	// Coordenada x do ponto final (termino no canto inferior direito).
	x = frame->getWidth();

	// Calculo o coeficiente angular da reta ('a' da equacao).
	a = ((float)frame->getHeight()/(float)frame->getWidth());
	
	/**
	 *  b = frame->getHeight porque eh onde a reta cruza o eixo y,
	 * considerando que a diagonal a ser analisada inicia-se a partir
	 * do canto superior esquerdo.
	**/
	b = frame->getHeight();

	// Vou pegar todos os pixels pertencentes à diagonal.
	// Equacao da reta.
	y = cvRound(a*column+b);

	Log::writeLog("%s :: get the pixel value", __FUNCTION__);
	
	luminance = ((uchar*)(frame->data->imageData + frame->data->widthStep*y))[column];	

	return(luminance);
				
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
	((uchar*)(this->data->imageData + this->data->widthStep*y))[x] = lum;
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
unsigned char Frame::getPixel(int x, int y)
{
	return ((uchar*)(this->data->imageData + this->data->widthStep*y))[x];
}

/************************************************************************
* Sobrecarga do operador '+' ele ira realizar a concatenacao dos frames
* Obs.: Atencao para os parametros, pois o soma tem que ser a dois objetos 
* absolutos, e nao dois ponteiros
*************************************************************************
* param (E): Frame frame -> Frame a ser concatenado
*************************************************************************
* Histórico:
* 14/07/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
Frame* Frame::operator+(Frame frame2)
{

	IplImage* img_dst;
	Frame *frameNew;

	int x, y;
	int x1, y1;

	// Primeiro precisamos criar um frame
	// que tenha como largura a soma das larguras dos dois frames
	// para altura usamos a do frame final
	img_dst = cvCreateImage(cvSize(this->getWidth() + frame2.getWidth(), this->getHeight()), 8, 1);
	frameNew = new Frame(img_dst);

	Log::writeLog("%s :: param: this[%x] frame2[%x]", __FUNCTION__, this->data, frame2.data);

	// Primeiro copiamos o frame de origem (this) para o novo frame
	for ( x = 0 ; x <= this->getWidth() ; x++)
	{
		for ( y = 0 ; y <= frame2.getHeight() ; y++ )
		{
			//Log::writeLog("Setting pixel x[%d],y[%d] = %d", x, y, this->getPixel(x,y));

			frameNew->setPixel(x, y, this->getPixel(x, y));
		}
	}

	// Agora concatenamos com o segundo frame passado por parametro
	for ( x1 = 0 ; x1 <= frame2.getWidth() ; x1++, x++)
	{
		for ( y1 = 0 , y = 0 ; y1 <= this->getHeight() ; y1++, y++)
		{
			//Log::writeLog("Setting pixel x[%d],y[%d] = %d from x1[%d],y1[%d]", x, y, frame2.getPixel(x1,y1), x1, y1);

			frameNew->setPixel(x, y, frame2.getPixel(x1, y1));
		}
	}

	Log::writeLog("%s :: return frameNew[%x]", __FUNCTION__, frameNew);

	return (frameNew);
}
