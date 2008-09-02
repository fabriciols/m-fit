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
	width =  vdo->getFramesWidth();
	heigth = vdo->getFramesHeight();
	frame =  vdo->getNextFrame();
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
		Log::writeLog("%s :: conver2Gray!", __FUNCTION__);
		frameGray = color->convert2Gray(frame);

		// Pego a diagonal (pixel por pixel) e ploto este pixel na coluna f do RV.
		Log::writeLog("%s :: getDiagonal!", __FUNCTION__);
		diagonal = frameGray->getDiagonal();


		// Gero o histograma da diagonal do frame corrente
		Log::writeLog("%s :: createHistogram!", __FUNCTION__);
		histogram = diagonal->createHistogram();
		//histogram = frameGray->createHistogram();

		// Guardo a media do valor de luminancia da diagonal.
		hist[posic] = (double)diagonal->mediaBin(diagonal);	
		//hist[posic] = (double)histogram->getMaxLuminance();	
	
		Log::writeLog("%s :: hist[%d] = %lf", __FUNCTION__, posic, hist[posic]);

		/**
		 * Deleto os objetos criados anteriormente para desalocamento de
		 * memoria.
		**/
		delete frameGray;
		delete histogram;
		delete frame;
		delete diagonal;
		
		// Pego o proximo frame e sua diagonal
		Log::writeLog("%s :: getNextFrame", __FUNCTION__);

		frame = vdo->getNextFrame();

		posic++;

		Log::writeLog("%s :: end end while", __FUNCTION__);
	}

	delete color;
	// Retorno o array com os valores do RVH

	return (hist);
}

/************************************************************************
* Fun��o que retira as tarjas widescreen do ritmo visual do v�deo.
*************************************************************************
* param (E): Frame* visualRythim : Ritmo visual a ser tratado
*************************************************************************
* return : Ritmo visual sem wide.
*************************************************************************
* Hist�rico:
* 01/09/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

void VisualRythim::removeWide(Frame* visualRythim)
{

	int height = 0;
	int width = 0;

	int sizeWide = 0;
	int oldSize = 0;
	int repete = 0;

	Frame* visual = new Frame();

	visual = visualRythim;

	height = visual->getHeight();
	width = visual->getWidth();

	for( int x=0; x<width; x++ )
	{
		// Quando encontrar o pixel diferente de preto eu entro e guardo a altura.
		for( int y=0; y<height || visual->getPixel(x,y) != 0; y++)
		{
			// Se logo o primeiro pixel n�o for preto, n�o tenho wide.
			if(y == 1)
			{
				sizeWide = 0;
				return;
			}

			sizeWide = y;
		}

		// Se a nova altura for igual � altura anterior, incremento um contador.
		if ( sizeWide == oldSize)
			repete++;

		oldSize = sizeWide;

		// Se a mesma altura se repetiu por metade do RV, significa que aquela � a altura 
		// do wide, ent�o n�o preciso mais analisar o resto. Mas se n�o se repetiu � porque
		// n�o existe wide entao zero o valor do wide.
		if (repete > height/2)
			break;
		else
			sizeWide = 0;

	}

	// Se houver widescreen
	if (sizeWide)
	{
		
		IplImage* img_dst;

		img_dst = Frame::imgAlloc(cvSize(width,(height-sizeWide)), visual->data->depth, visual->data->nChannels);

		// Pego somente a parte de interesse (sem o wide)
		cvSetImageROI(img_dst,
			cvRect
			(
			 0,
			 sizeWide,
			 visual->getWidth(),
			 (visual->getHeight()-sizeWide))
			);

		(visual->data,img_dst);
	}
}
