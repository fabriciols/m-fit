#include "cv.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"

#include "../include/Morphology.h"

/************************************************************************
* Fun��o que aplica o efeito de Dilata��o no Frame
*************************************************************************
* param (E): frame  - Frame onde o efeito vai ser aplicado
* return   : Frame* - Ponteiro para um novo frame com o efeito aplicado
************************************************************************
* Hist�rico:
* 26/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Frame* Morphology::dilate(Frame* frame)
{
	IplImage *imgDst;

	imgDst = cvCreateImage(cvGetSize(frame->data),frame->data->depth,frame->data->nChannels);

	cvDilate(frame->data, imgDst, 0, 1);

	return (new Frame(imgDst));
}

/************************************************************************
* Fun��o que aplica o efeito de Eros�o no Frame
*************************************************************************
* param (E): frame  - Frame onde o efeito vai ser aplicado
* return   : Frame* - Ponteiro para um novo frame com o efeito aplicado
************************************************************************
* Hist�rico:
* 26/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Frame* Morphology::erode(Frame* frame)
{
	IplImage *imgDst;

	imgDst = cvCreateImage(cvGetSize(frame->data),frame->data->depth,frame->data->nChannels);

	cvErode(frame->data, imgDst, 0, 1);

	return (new Frame(imgDst));
}
