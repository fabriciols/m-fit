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
* 06/07/08 - Thiago Mizutani
* Implementação do processo que monta as colunas do ritmo visual atraves
* da extracao dos pixels da diagonal de um frame.
* 04/07/08 - Thiago Mizutani
* Criação.
************************************************************************/

Frame* VisualRythim::createVR(Video* vdo)
{

	IplImage* visualRythim; // Imagem do ritmo visual.	
	Color* color;

	int f = 0; // nro do frame corrente.
	int max_frames = vdo->totalFrames; // Numero total de frames do video.
	int x = 0; // Coluna que está sendo montada no RV.

	// Pego o primeiro frame do video.
	Frame frame = new Frame(vdo->getCurrentFrame(this));

	/**
	 *  Crio a imagem do RV com largura = ao nro de frames do video (cada
	 *  frame representa 1 coluna do RV) e altura = largura do frame.
   **/
	visualRythim = cvCreateImage(cvSize(max_frames,frame->width),8,1);

	Frame vr = new Frame(visualRythim);

	for (f=0;f<max_frames;++f)
	{
		// Converto o frame para escala de cinza.
		frame = color->convert2Gray(frame);

		// Pego a diagonal do frame.
		for (x=0;x<frame->getWidth;x++)
		{
			// Pego a diagonal (pixel por pixel) e ploto este pixel na coluna f do RV.
    		((uchar*)(vr->data->imageData + vr->data->widthStep*x))[f] = frame->getDiagonal(this,x);
		}
	
		// Pego um novo frame.	
		frame->data = vdo->getNextFrame(this);

		// Se não houver mais frames, chegou ao fim do video.
		if (!frame)
			break;
	}

	return (vr);
}
