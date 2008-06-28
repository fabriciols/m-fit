#include "cv.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Filters.h"

/************************************************************************
* Função que realiza a segmentacao da imagem usando um limiar.
*************************************************************************
* param (E): frame     - Frame a ser aplicado o efeito 
* param (E): threshold - Valor do limiar a ser aplicado
* return   : Frame*    - Ponteiro para um novo frame com o efeito
************************************************************************
* Histórico:
* 26/06/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
Frame* Filters::segment(Frame* frame, int threshold)
{
	IplImage* img_dst;

	// Faz uma copia da imagem
	img_dst = cvCreateImage(cvGetSize(frame->data),frame->data->depth,frame->data->nChannels);

	// Aplica o limiar
	cvThreshold(frame->data, img_dst, threshold, 255, CV_THRESH_BINARY_INV);

	// Retorna um objeto encapsulado do tipo frame
	return (new Frame(img_dst));
}

/************************************************************************
* Função que aplica o Operador Sobel nas imagens.
*************************************************************************
* param (E): Frame* frame: Imagem à qual será aplicado o operador.
* param (E): int direction:
* 				 0 - Vertical
* 				 1 - Horizontal
* 				 2 - Vertical e Horizontal
************************************************************************
* Retorno: Frame com o efeito aplicado (img_dst)
************************************************************************
* Autor: Thiago Mizutani
************************************************************************
* Histórico:
* 23/06/08 - Thiago Mizutani
* Criação.
************************************************************************/

Frame* Filters::Sobel(Frame* frame, int direction)
{
   IplImage* img_dst;
	
	img_dst = cvCreateImage(cvGetSize(frame->data),frame->data->depth,frame->data->nChannels);
   
   switch(direction)
   {
      case 0: //Vertical
         //origem, destino, vert, horiz, abertura.
         cvSobel(frame->data,img_dst,1,0,3);
         break;
      case 1: //Horizontal
         cvSobel(frame->data,img_dst,0,1,3);
			break;
      case 2: //Both
         cvSobel(frame->data,img_dst,1,1,3);
			break;
   }
   
   return(new Frame(img_dst));
}

/************************************************************************
* Função que aplica o filtro passa-baixa na imagem.
*************************************************************************
* param (E): Frame* frame: Imagem à qual será aplicado o operador.
* param (E): int size: Tamanho da matriz.
* 				 3 - 3x3
* 				 5 - 5x5
* 				 7 - 7x7
* 				 n - nxn
************************************************************************
* Retorno: Frame com o efeito aplicado (img_dst)
************************************************************************
* Autor: Thiago Mizutani
************************************************************************
* Histórico
* 24/06/08 - Thiago Mizutani
* Alteração da montagem do kernel do filtro.
* 23/06/08 - Thiago Mizutani
* Criação.
************************************************************************/

Frame* Filters::lowPass(Frame* frame, int size)
{
   IplImage* imgDst = 0;
	IplImage* imgAux = 0;

	//Ajuste do tamanho da matriz.
	if (size >= 11)
		size = 9;

	int cols_i = size;
	int rows_i = size;
	int total_size = 0;
	CvMat *filter = 0;

	total_size=(int)pow(size,2);

	// Máscara para realizar o processo de convolução.
	double convMask[total_size];
	
	// Cria uma imagem com os mesmos parâmetros da original.
	imgDst = cvCreateImage(cvGetSize(frame->data), frame->data->depth, frame->data->nChannels);
	imgAux = cvCreateImage(cvGetSize(frame->data), frame->data->depth, frame->data->nChannels);

	// Monta a máscara com o tamanho que foi passado como parâmetro.
	for (int i=0; i<total_size; i++)	
		convMask[i] = (double)1/(double)total_size;

	imgAux->imageData = frame->data->imageData;
	imgAux->widthStep = frame->data->width;

	imgDst->imageData = imgAux->imageData;
	imgDst->widthStep = imgAux->width;

	filter = cvCreateMatHeader(rows_i, cols_i, CV_64FC1);

	cvSetData(filter, convMask, cols_i*8);

	cvFilter2D(imgAux, imgDst, filter, cvPoint(-1,-1));

	return (new Frame(imgDst));

}

/************************************************************************
* Função que aplica o filtro passa-alta na imagem.
*************************************************************************
* param (E): Frame* frame: Imagem à qual será aplicado o operador.
* param (E): int typeMas: Tipo da máscara a ser utilizada.
************************************************************************
* Retorno: Frame com o efeito aplicado (img_dst)
************************************************************************
* Autor: Thiago Mizutani
************************************************************************
* Histórico
* 29/06/08 - Thiago Mizutani
* Criação.
************************************************************************/

Frame* Filters::highPass(Frame* frame, int typeMask)
{
	IplImage* imgDst = 0;
	IplImage* imgAux = 0;

	int cols = 0;
	int rows = 0;
	CvMat *filter = 0;

	// Máscaras de convolução
	double masks[][9] = {
		// Tipo 1
		{ 
			-1, -1, -1,
			-1,  8, -1,
			-1, -1, -1,
		},
		// Tipo 2
		{
			0, -1,  0,
			-1, 4, -1,
			0, -1,  0,
		},
		// Tipo 3
		{
			1 , -2,  1,
			-2,  4, -2,
			1 , -2,  1,
		},
	};

	cols = 3;
	rows = 3;

//	mask = masks[typeMask];

	sprintf(effectName, "High-Pass kernel [%d]", typeMask);

	imgAux->imageData = frame->data->imageData;
	imgAux->widthStep = frame->data->width;

	imgDst->imageData = imgAux->imageData;
	imgDst->widthStep = imgAux->width;

	filter = cvCreateMatHeader(rows_i, cols_i, CV_64FC1);

	cvSetData(filter, masks[typeMask], cols_i*8);

	cvFilter2D(imgAux, imgDst, filter, cvPoint(-1,-1));

	return (new Frame(imgDst));
}
