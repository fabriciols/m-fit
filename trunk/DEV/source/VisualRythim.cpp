#include "cv.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/VisualRythim.h"

/************************************************************************
* Fun��o que cria o Ritmo Visual por amostragem de um v�deo.
*************************************************************************
* param (E): Video* vdo => v�deo do qual ser� montado o ritmo visual.
*************************************************************************
* return : Ritmo Visual do v�deo.
*************************************************************************
* Hist�rico:
* 04/07/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

IplImage* VisualRythim::createVR(Video* vdo)
{
	Frame* vr = 0;
	
	/**
	 * Visto que os frames de um mesmo video nao tem suas dimensoes
	 * alteradas, frame->heigth e frame->width s�o fixos para todos
	 * os objetos, ent�o s� altero o frame->data.
	**/	
	Frame frame = new Frame(cvCaptureFrame(vdo));
	
	vr = cvCreateImage(frame->)
	
	while(!frame)
	{
		// Pego a diagonal do frame.
		vr->getDiagonal(frame);
	
		// Pego um novo frame.	
		frame->data = cvCaptureFrame(vdo); // trocar por getNextFrame

		// Se n�o houver mais frames, chegou ao fim do video.
		if (!frame)
			break;
	}

	return (vr->data);
}
