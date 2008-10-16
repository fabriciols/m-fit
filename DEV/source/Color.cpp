#include "cv.h"
#include <QImage>

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Effect.h"
#include "../include/Color.h"

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
Frame* Color::convert2Gray(Frame* frame)
{
	IplImage *imgGray;
	Frame *frameNew;

	// Cria a img com as mesmas dimens�es da inst�ncia atual
	// por�m com 8 bits e 1 canal apenas
	imgGray = Frame::imgAlloc(cvGetSize(frame->data),8,1);

	// Converte pra cinza;
	cvCvtColor(frame->data, imgGray, CV_RGB2GRAY);

	frameNew = new Frame(imgGray);

	Frame::imgDealloc(imgGray);

	return (frameNew);
}

Frame* Color::applyEffect(Frame* frame)
{
	return convert2Gray(frame);
}

Color::Color(long frameStart, long frameEnd)
{
	this->frameStart = frameStart;
	this->frameEnd = frameEnd;
}

Color::Color()
{
	this->frameStart = 0;
	this->frameEnd = 0;
}
