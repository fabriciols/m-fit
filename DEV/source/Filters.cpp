#include <stdio.h>
#include <stdlib.h>
#include "../include/Filters.h"

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
* Parâmetros:
* Frame* frame: Imagem à qual será aplicado o operador.
* int direction:
* 0 - Vertical
* 1 - Horizontal
* 2 - Both
************************************************************************
* Retorno: Frame com o efeito aplicado (img_dst)
************************************************************************
* Autor: Thiago Mizutani
* 23/06/08 - Criação.
************************************************************************/

Frame* Filters::Sobel(Frame* frame, int direction)
{
   IplImage* img_dst;
   
   switch(direction)
   {
      case 0: //Vertical
         //origem, destino, vert, horiz, abertura.
         cvSobel(frame->data,img_dst,1,0,3);
         break;
      case 1: //Horizontal
         cvSobel(frame->data,img_dst,0,1,3);
      case 2: //Both
         cvSobel(frame->data,img_dst,1,1,3);
   }
   
   return(new Frame(img_dst));
}

/************************************************************************
* Função que aplica o filtro passa-baixa na imagem.
*************************************************************************
* Parâmetros:
* Frame* frame: Imagem à qual será aplicado o operador.
* int size: Tamanho da matriz.
* 3 - 3x3
* 5 - 5x5
* 7 - 7x7
* n - nxn
************************************************************************
* Retorno: Frame com o efeito aplicado (img_dst)
************************************************************************
* Autor: Thiago Mizutani
* 23/06/08 - Criação.
************************************************************************/

Frame* Frame::lowPass(Frame* frame, int size)
{
   IplImage* img_dst = 0;

	// Tamanho da Matriz
	int cols_i = size;
	int rows_i = size;
	int total_size = 0;
	double *kernel;
	CvMat *filter = 0;
	
	total_size=pow(size,2);
	
	double kernel[size][size];
	
	for (int i=0;i<size;i++)
      for(int j=0;j<size;j++)
         kernel[i][j] = 1/total_size;

   img_dst = cvCreateImageHeader(cvGetSize(frame), 8, 1);
	img_dst->imageData = imgAux->imageData;
	img_dst->widthStep = imgAux->width;

	filter = cvCreateMatHeader(rows_i, cols_i, CV_64FC1);

	cvSetData(filter, kernel, cols_i*8);

	cvFilter2D(frame, img_dst, filter, cvPoint(-1,-1));

}
