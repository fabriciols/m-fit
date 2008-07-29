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
* Função que cria o Ritmo Visual por amostragem de um vídeo.
*************************************************************************
* param (E): Video* vdo => vídeo do qual será montado o ritmo visual.
*************************************************************************
* return : Ritmo Visual do vídeo.
*************************************************************************
* Histórico:
* 29/07/08 - Thiago Mizutani
* Retirando coisas obsoletas.
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
	Frame *frameAux;

	Color *color;

	int lum;

	double width;
	double heigth;
	int x = 0; // Coluna que está sendo montada no RV.

	Log::writeLog("%s :: param: Video[%x]", __FUNCTION__, vdo);

	color = new Color();

	// Pego o primeiro frame do vídeo e sua diagonal	
	width = vdo->getFramesWidth();
	heigth = vdo->getFramesHeight();
	frame = vdo->getNextFrame();
	frameVR = new Frame(frame->getDiagonal());

	while(frame != NULL)
	{

		Log::writeLog("%s :: Convert2Gray [%x]", __FUNCTION__, frame);

		// Converto o frame para escala de cinza.
		frame = color->convert2Gray(frame);

		// Pego a diagonal (pixel por pixel) e ploto este pixel na coluna f do RV.
		frameAux = frame->getDiagonal();

		/** Utilizando sobrecarga de operador para concatenar uma nova coluna ao frame
		 * do Ritmo Visual.
		**/

		*frameVR += *frameAux;

		// Desaloca os temporarios
		delete frameAux;
		delete frame;
		
		frame = vdo->getNextFrame();
	}

	Log::writeLog("%s :: max_frames[%.f]", __FUNCTION__, vdo->getFramesTotal());

	Log::writeLog("%s :: end ", __FUNCTION__);

	return (frameVR);
}

/************************************************************************
* Função que cria o Ritmo Visual por histograma de um vídeo.
*************************************************************************
* param (E): Video* vdo => vídeo do qual será montado o ritmo visual.
*************************************************************************
* return : Ritmo Visual por histograma do vídeo.
*************************************************************************
* Histórico:
* 29/07/08 - Thiago Mizutani
* Criação.
************************************************************************/

int* VisualRythim::createVRH(Video* vdo)
{
	Frame* frameVRH = 0;	// Ritmo visual por histograma
	Frame* frame = 0; 
	Frame* diagonal = 0;
	
	Histogram* histogram = 0;	

	double totalFrames = vdo->getFramesTotal();
	int posic = 0;

	/**
	 * Crio um array com o numero de posicoes = ao nro de frames do video
	 * para guardar o valor máximo de cada histograma que será gerado.
	 * Obs.: Será gerado 1 histograma por frame.
	**/
	int* hist;

	hist = (int*)malloc(sizeof(int)*cvRound(totalFrames));
	memset(hist,'0',cvRound(totalFrames));

	// Pego o primeiro frame e sua diagonal

	frame = vdo->getNextFrame();	
	diagonal = new Frame(frame->getDiagonal());
	
	while(!frame)
	{
		// Gero o histograma da diagonal do frame corrente
		frame->createHistogram(diagonal->data, histogram);

		// Guardo o maior valor de luminancia do histograma.	
		hist[posic] = histogram->getMaxLuminance();	
	
		Log::writeLog("%s :: hist[%d] = %d", __FUNCTION__, posic, hist[posic]);

		/**
		 * Deleto os objetos criados anteriormente para desalocamento de
		 * memoria.
		**/
		delete histogram;
		delete frame;
		delete diagonal;
		
		// Pego o proximo frame e sua diagonal
		frame = vdo->getNextFrame();
		diagonal = frame->getDiagonal();

		posic++;
	}

	// Retorno o array com os valores do RVH
	return (hist);
}
