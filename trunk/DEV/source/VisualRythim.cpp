#include "cv.h"
#include "highgui.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/VisualRythim.h"

#include "../include/Effect.h"
#include "../include/Color.h"

/************************************************************************
* Fun��o que cria o Ritmo Visual por amostragem de um v�deo.
*************************************************************************
* param (E): Video* vdo => v�deo do qual ser� montado o ritmo visual.
*************************************************************************
* return : Ritmo Visual do v�deo.
*************************************************************************
* Hist�rico:
* 06/07/08 - Thiago Mizutani
* Implementa��o do processo que monta as colunas do ritmo visual atraves
* da extracao dos pixels da diagonal de um frame.
* 04/07/08 - Thiago Mizutani
* Cria��o.
************************************************************************/
Frame* VisualRythim::createVR(Video* vdo)
{

	Frame *vr;
	Color* color;

	int f = 0; // nro do frame corrente.
	double max_frames = vdo->getFramesTotal(); // Numero total de frames do video.
	int x = 0; // Coluna que est� sendo montada no RV.

	// Pego o primeiro frame do video.
	Frame *frame = new Frame(vdo->getCurrentFrame());

	/**
	 *  Crio a imagem do RV com largura = ao nro de frames do video (cada
	 *  frame representa 1 coluna do RV) e altura = largura do frame.
   **/
	vr = new Frame(cvCreateImage(cvSize((int)max_frames,frame->getWidth()),8,1));

	for (f=0;f<max_frames;++f)
	{
		// Converto o frame para escala de cinza.
		frame = color->convert2Gray(frame);

		// Pego a diagonal do frame.
		for (x=0;x<frame->getWidth();x++)
		{
			// Pego a diagonal (pixel por pixel) e ploto este pixel na coluna f do RV.
    		((uchar*)(vr->data->imageData + vr->data->widthStep*x))[f] = frame->getDiagonal(frame, x);
		}
	
		// Pego um novo frame.	
		frame = vdo->getNextFrame();

		// Se n�o houver mais frames, chegou ao fim do video.
		if (!frame)
			break;
	}

	return (vr);
}
