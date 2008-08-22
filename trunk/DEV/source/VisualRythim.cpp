#include "cv.h"
#include "highgui.h"

#include "../include/Time.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/VisualRythim.h"

#include "../include/Effect.h"
#include "../include/Color.h"

#include "../include/Log.h"

#include <stdio.h>

/************************************************************************
* Fun��o que cria o Ritmo Visual por amostragem de um v�deo.
*************************************************************************
* param (E): Video* vdo => v�deo do qual ser� montado o ritmo visual.
*************************************************************************
* return : Ritmo Visual do v�deo.
*************************************************************************
* Hist�rico:
* 29/07/08 - Thiago Mizutani
* Retirando coisas obsoletas.
* 06/07/08 - Thiago Mizutani
* Implementa��o do processo que monta as colunas do ritmo visual atraves
* da extracao dos pixels da diagonal de um frame.
* 04/07/08 - Thiago Mizutani
* Cria��o.
************************************************************************/
Frame* VisualRythim::createVR(Video* vdo)
{

	Frame *frameVR = 0;
	Frame* frame = 0;
	Frame* frameGray = 0;
	Frame *frameAux = 0;

	Color *color = 0;

	int lum;

	double width;
	double heigth;
	int x = 0; // Coluna que est� sendo montada no RV.

	Log::writeLog("%s :: param: Video[%x]", __FUNCTION__, vdo);

	color = new Color();

	// Pego o primeiro frame do v�deo e sua diagonal	
	width = vdo->getFramesWidth();
	heigth = vdo->getFramesHeight();
	frame = vdo->getNextFrame();
	frameVR = new Frame(frame->getDiagonal());

	while(true)
	{
		frame = vdo->getNextFrame();

		if (frame == NULL)
			break;

		Log::writeLog("%s :: Convert2Gray Frame[%x] Frame->data[%x]", __FUNCTION__, frame, frame->data);

		// Converto o frame para escala de cinza.
		// INCLUIR ESTE PASSO NO DIAGRAMA DE SEQUENCIA!!!!!!!!!!!!!
		frameGray = color->convert2Gray(frame);

		// Pego a diagonal (pixel por pixel) e ploto este pixel na coluna f do RV.
		frameAux = frameGray->getDiagonal();

		/** Utilizando sobrecarga de operador para concatenar uma nova coluna ao frame
		 * do Ritmo Visual.
		**/

		*frameVR += *frameAux;

		// Desaloca os temporarios
		delete frameAux;
		delete frame;
		delete frameGray;

	}

	Log::writeLog("%s :: max_frames[%.f]", __FUNCTION__, vdo->getFramesTotal());
	return (frameVR);
}

/************************************************************************
* Fun��o que cria o Ritmo Visual por histograma de um v�deo.
*************************************************************************
* param (E): Video* vdo => v�deo do qual ser� montado o ritmo visual.
*************************************************************************
* return : Ritmo visual por histograma (um array).
*************************************************************************
* Hist�rico:
* 18/08/08 - Ivan Shiguenori Machida
* Convertendo o frame para tons de cinza antes de obter a diagonal
* 29/07/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

double* VisualRythim::createVRH(Video* vdo)
{
	Frame* frame = new Frame(); 
	Frame* diagonal = new Frame();
	Histogram* histogram = new Histogram();	
	Color *color = new Color();
	Frame* frameGray = 0;

	double totalFrames = vdo->getFramesTotal();
	int posic = 0;

	/**
	 * Crio um array com o numero de posicoes = ao nro de frames do video
	 * para guardar o valor m�ximo de cada histograma que ser� gerado.
	 * Obs.: Ser� gerado 1 histograma por frame.
	**/
	double* hist; // Este � o RVH.

	hist = (double*)malloc(sizeof(double)*cvRound(totalFrames));

	memset(hist, '\0',     sizeof(double)*cvRound(totalFrames));

	// Pego o primeiro frame e sua diagonal
	frame = vdo->getNextFrame();	
	
	while(frame != NULL)
	{

		if (posic >= totalFrames)
		{
			Log::writeLog("%s :: BOOOOOOM!", __FUNCTION__);
		}

		// Converto o frame para escala de cinza.
		// INCLUIR ESTE PASSO NO DIAGRAMA DE SEQUENCIA!!!!!!!!!!!!!
		frameGray = color->convert2Gray(frame);

		// Pego a diagonal (pixel por pixel) e ploto este pixel na coluna f do RV.
		diagonal = frameGray->getDiagonal();

		Log::writeLog("%s :: createHistogram", __FUNCTION__);

		// Gero o histograma da diagonal do frame corrente
		histogram = diagonal->createHistogram();

		// Guardo o maior valor de luminancia do histograma.	
//		hist[posic] = (double)histogram->getMaxLuminance();	

		// Guardo a media do valor de luminancia da diagonal.
		hist[posic] = (double)diagonal->mediaBin(diagonal);	
	
		Log::writeLog("%s :: hist[%d] = %lf", __FUNCTION__, posic, hist[posic]);

		/**
		 * Deleto os objetos criados anteriormente para desalocamento de
		 * memoria.
		**/
		delete histogram;
		delete frame;
		delete diagonal;
		
		// Pego o proximo frame e sua diagonal
		Log::writeLog("%s :: getNextFrame", __FUNCTION__);

		frame = vdo->getNextFrame();

		posic++;

		Log::writeLog("%s :: end while", __FUNCTION__);
	}

	Log::writeLog("%s :: end", __FUNCTION__);

	delete color;
	// Retorno o array com os valores do RVH
	return (hist);
}
