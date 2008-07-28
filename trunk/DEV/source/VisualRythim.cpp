#include "cv.h"
#include "highgui.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/VisualRythim.h"

#include "../include/Effect.h"
#include "../include/Color.h"

#include "../include/Log.h"

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

	Frame *frameVR;

	Frame* frame;

	Color *color;
	IplImage* imgVR;

	Frame *frameAux;

	int lum;

	double width;
	double heigth;
	int f = 0; // nro do frame corrente.
	int x = 0; // Coluna que está sendo montada no RV.
	double max_frames; // Numero total de frames do video.
	int diagonal;

	Log::writeLog("%s :: param: Video[%x]", __FUNCTION__, vdo);

	color = new Color();

	// Pego o primeiro frame do video.
	width = vdo->getFramesWidth();
	heigth = vdo->getFramesHeight();
	max_frames = vdo->getFramesTotal(); // Numero total de frames do video.

	diagonal = (int)cvRound(sqrt( pow(width,2) + pow(heigth,2)));

	/**
	 *  Crio a imagem do RV com largura = ao nro de frames do video (cada
	 *  frame representa 1 coluna do RV) e altura = largura do frame.
   **/

	imgVR = cvCreateImage(cvSize((int)1, (int)diagonal), 8, 1);

	frame = vdo->getNextFrame();

	frameVR = new Frame(frame->getDiagonal());

	f = 0;

	for (f=0 ; f< max_frames ; f++)
	{

		frame = vdo->getNextFrame();

		// Se não houver mais frames, chegou ao fim do video.
		if (!frame)
			break;

		Log::writeLog("%s :: Convert2Gray [%x]", __FUNCTION__, frame);

		// Converto o frame para escala de cinza.
		frame = color->convert2Gray(frame);

		// Pego a diagonal (pixel por pixel) e ploto este pixel na coluna f do RV.
		frameAux = frame->getDiagonal();

		*frameVR += *frameAux;

		// Desaloca os temporarios
		delete frameAux;
		delete frame;
	}

	Log::writeLog("%s :: frame[%x] max_frames[%.f] diagonal[%d]", __FUNCTION__, imgVR, max_frames, diagonal);

	Log::writeLog("%s :: end ", __FUNCTION__);

	return (frameVR);
}
