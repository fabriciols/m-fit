#include "cv.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/VisualRythim.h"

/************************************************************************
* Função que cria o Ritmo Visual por amostragem de um vídeo.
*************************************************************************
* param (E): Video* vdo => vídeo do qual será montado o ritmo visual.
*************************************************************************
* return : Ritmo Visual do vídeo.
*************************************************************************
* Histórico:
* 04/07/08 - Thiago Mizutani
* Criação.
************************************************************************/

IplImage* VisualRythim::createVR(Video* vdo)
{
	Frame* vr = 0;
	
	/**
	 * Visto que os frames de um mesmo video nao tem suas dimensoes
	 * alteradas, frame->heigth e frame->width são fixos para todos
	 * os objetos, então só altero o frame->data.
	**/	
	Frame frame = new Frame(cvCaptureFrame(vdo));
	
	vr = cvCreateImage(frame->)
	
	while(!frame)
	{
		// Pego a diagonal do frame.
		vr->getDiagonal(frame);
	
		// Pego um novo frame.	
		frame->data = cvCaptureFrame(vdo); // trocar por getNextFrame

		// Se não houver mais frames, chegou ao fim do video.
		if (!frame)
			break;
	}

	return (vr->data);
}
