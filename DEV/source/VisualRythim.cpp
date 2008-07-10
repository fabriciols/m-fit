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

	Frame *vr;
	Color *color;
	IplImage* img;
	Frame* frame;

	int lum;

	double width;
	int f = 0; // nro do frame corrente.
	int x = 0; // Coluna que está sendo montada no RV.
	double max_frames; // Numero total de frames do video.

	Log::writeLog("%s :: param: Video[%x]", __FUNCTION__, vdo);

	color = new Color();

	// Pego o primeiro frame do video.
	width = vdo->getFramesWidth();
	max_frames = vdo->getFramesTotal(); // Numero total de frames do video.

	/**
	 *  Crio a imagem do RV com largura = ao nro de frames do video (cada
	 *  frame representa 1 coluna do RV) e altura = largura do frame.
   **/
	Log::writeLog("%s :: frame[%x] max_frames[%.f]", __FUNCTION__, img, max_frames);

	img = cvCreateImage(cvSize((int)max_frames,(int)width),8,1);

	vr = new Frame(img);

	for (f=0;f<max_frames;++f)
	{

		frame = vdo->getNextFrame();

		// Se não houver mais frames, chegou ao fim do video.
		if (!frame)
			break;

		Log::writeLog("%s :: Convert2Gray [%x]", __FUNCTION__, frame);

		// Converto o frame para escala de cinza.
		frame = color->convert2Gray(frame);

		// Pego a diagonal do frame.
		for (x=0;x<frame->getWidth();x++)
		{

			Log::writeLog("%s :: get lum", __FUNCTION__);

			// Pego a diagonal (pixel por pixel) e ploto este pixel na coluna f do RV.
    		lum = frame->getDiagonal(frame, x);

			Log::writeLog("%s :: Diagonal x[%d] width[%d] lum [%d] frame[%x]", __FUNCTION__, x, frame->getWidth(), lum, frame);

    		((uchar*)(vr->data->imageData + vr->data->widthStep*x))[f] = lum;

			Log::writeLog("%s :: writePixel", __FUNCTION__);
		}
	
	}

	return (vr);
}
