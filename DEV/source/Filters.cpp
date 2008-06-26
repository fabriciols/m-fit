#include "cv.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Filters.h"

/************************************************************************
* Fun��o que realiza a segmentacao da imagem usando um limiar.
*************************************************************************
* param (E): frame     - Frame a ser aplicado o efeito 
* param (E): threshold - Valor do limiar a ser aplicado
* return   : Frame*    - Ponteiro para um novo frame com o efeito
* aplicado.
************************************************************************
* Hist�rico:
* 26/06/08 - Fabricio Lopes de Souza
* Cria��o.
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
* Fun��o que aplica o Operador Sobel nas imagens.
*************************************************************************
* Par�metros:
* Frame* frame: Imagem � qual ser� aplicado o operador.
* int direction:
* 0 - Vertical
* 1 - Horizontal
* 2 - Both
************************************************************************
* Retorno: Frame com o efeito aplicado (img_dst)
************************************************************************
* Autor: Thiago Mizutani
************************************************************************
* Hist�rico:
* 23/06/08 - Thiago Mizutani
* Cria��o.
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
* Fun��o que aplica o filtro passa-baixa na imagem.
*************************************************************************
* Par�metros:
* Frame* frame: Imagem � qual ser� aplicado o operador.
* int size: Tamanho da matriz.
* 3 - 3x3
* 5 - 5x5
* 7 - 7x7
* n - nxn
************************************************************************
* Retorno: Frame com o efeito aplicado (img_dst)
************************************************************************
* Autor: Thiago Mizutani
************************************************************************
* Hist�rico
* 24/06/08 - Thiago Mizutani
* Altera��o da montagem do kernel do filtro.
* 23/06/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

Frame* Filters::lowPass(Frame* frame, int size)
{
   IplImage* imgDst = 0;
	IplImage* imgAux = 0;

	// Tamanho da Matriz
	
	//size = size >= 11 ? 9 : size;
	
	if (size >= 11)
		size = 9;

	int cols_i = size;
	int rows_i = size;
	int total_size = 0;
	CvMat *filter = 0;

	total_size=(int)pow(size,2);

	// M�scara para realizar o processo de convolu��o.
	double kernel[total_size];
	
	// Cria uma imagem com os mesmos par�metros da original.
	imgDst = cvCreateImage(cvGetSize(frame->data), frame->data->depth, frame->data->nChannels);
	imgAux = cvCreateImage(cvGetSize(frame->data), frame->data->depth, frame->data->nChannels);

	// Monta a m�scara com o tamanho que foi passado como par�metro.
	for (int i=0; i<total_size; i++)	
		kernel[i] = (double)1/(double)total_size;

	imgAux->imageData = frame->data->imageData;
	imgAux->widthStep = frame->data->width;

	imgDst->imageData = imgAux->imageData;
	imgDst->widthStep = imgAux->width;

	filter = cvCreateMatHeader(rows_i, cols_i, CV_64FC1);

	cvSetData(filter, kernel, cols_i*8);

	cvFilter2D(imgAux, imgDst, filter, cvPoint(-1,-1));

	return (new Frame(imgDst));

}
