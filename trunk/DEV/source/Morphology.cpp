#include "cv.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"

#include "../include/Morphology.h"

/************************************************************************
* Função que aplica o efeito de Dilatação no Frame
*************************************************************************
* param (E): frame  - Frame onde o efeito vai ser aplicado
* return   : Frame* - Ponteiro para um novo frame com o efeito aplicado
************************************************************************
* Histórico:
* 26/06/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
Frame* Morphology::dilate(Frame* frame)
{
	IplImage *imgDst;

	imgDst = cvCreateImage(cvGetSize(frame->data),frame->data->depth,frame->data->nChannels);

	cvDilate(frame->data, imgDst, 0, 1);

	return (new Frame(imgDst));
}

/************************************************************************
* Função que aplica o efeito de Erosão no Frame
*************************************************************************
* param (E): frame  - Frame onde o efeito vai ser aplicado
* return   : Frame* - Ponteiro para um novo frame com o efeito aplicado
************************************************************************
* Histórico:
* 26/06/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
Frame* Morphology::erode(Frame* frame)
{
	IplImage *imgDst;

	imgDst = cvCreateImage(cvGetSize(frame->data),frame->data->depth,frame->data->nChannels);

	cvErode(frame->data, imgDst, 0, 1);

	return (new Frame(imgDst));
}
