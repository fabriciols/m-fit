#include "cv.h"
#include "highgui.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/VisualRythim.h"

#include "../include/Effect.h"
#include "../include/Color.h"

/************************************************************************
* Função que cria o Ritmo Visual por amostragem de um vídeo.
*************************************************************************
* param (E): Video* vdo => vídeo do qual será montado o ritmo visual.
*************************************************************************
* return : Ritmo Visual do vídeo.
*************************************************************************
* Histórico:
* 06/07/08 - Thiago Mizutani
* Implementação do processo que monta as colunas do ritmo visual atraves
* da extracao dos pixels da diagonal de um frame.
* 04/07/08 - Thiago Mizutani
* Criação.
************************************************************************/
Frame* VisualRythim::createVR(Video* vdo)
{

	Frame *vr;
	Color* color;

	int f = 0; // nro do frame corrente.
	double max_frames = vdo->getFramesTotal(); // Numero total de frames do video.
	int x = 0; // Coluna que está sendo montada no RV.

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

		// Se não houver mais frames, chegou ao fim do video.
		if (!frame)
			break;
	}

	return (vr);
}
