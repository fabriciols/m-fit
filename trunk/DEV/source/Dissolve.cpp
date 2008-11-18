#include "cv.h"
#include "highgui.h"
#include <stdexcept>
#include <vector>
#include <QImage>

#include "../include/Time.h"
#include "../include/Log.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"

#include "../include/Video.h"
#include "../include/Transition.h"
#include "../include/DetectTransitions.h"
#include "../include/VisualRythim.h"

#include "../include/Dissolve.h"
#include "../include/Effect.h"
#include "../include/Color.h"
#include "../include/Filters.h"

#define RIGHT	1
#define LEFT	2

/************************************************************************
* Função que detecta as transições do tipo Dissolve
*************************************************************************
* param (E): Video* vdo -> video à detectar as transições
* param (S): Transition* transitions -> Posicão corrente da lista de transições
*************************************************************************
* return : Nenhum
*************************************************************************
* Histórico:
* 16/10/08 - Mauricio Hirota / Ivan Shiguenori Machida
* Criação.
* 08/11/08 - Ivan Shiguenori Machida
* Utilizacao da funcao calcFirstDerivative
************************************************************************/
void Dissolve::detectTransitions(Video* vdo, std::vector<Transition>* transitionList)
{
	double *array_vrh;
	double *array_variance;
	int k;
	double videoSize;

	double *detect, *orientation, thresholdMax=0, thresholdMin=0;

	// Array da segunda derivada
	double *arrayDDvariance;
	// Array da primeira derivada
	double *arrayDmean;

	VisualRythim* vrh;

	videoSize = cvRound(vdo->getFramesTotal());

	detect = (double *) malloc(sizeof(double)*cvRound(videoSize));
	orientation = (double *) malloc(sizeof(double)*cvRound(videoSize));

	memset(detect, '\0',     sizeof(double)*cvRound(videoSize));
	memset(orientation, '\0',     sizeof(double)*cvRound(videoSize));

	vdo->removeWide();
	vdo->removeBorder();

	//Coleta o ritmo visual dos frames
	array_vrh = vrh->createVRH(vdo);
	
	vdo->seekFrame(0);
	
	array_variance = calcVariance(vdo);

	arrayDmean = calcDerivative(array_vrh, cvRound(videoSize));
	arrayDDvariance = calcDerivative(calcDerivative(array_variance, cvRound(videoSize)), cvRound(videoSize));

	thresholdMin = 0.25;
	thresholdMax = 4.0;

	//Verifica ponto de dissolve
	for(k=0;k<videoSize;k++)
	{
		if(arrayDDvariance[k]/arrayDmean[k] == 0)
		{
			detect[k]=1;

			while(arrayDDvariance[k]/arrayDmean[k] == 0)
				k++;
		}
	}

	//Transportando para matriz de detecção
	for(k=0;k<videoSize;k++)
	{
		if(detect[k]==1)
		{
			Transition *transition = new Transition();

			transition = new Transition(TRANSITION_DISSOLVE, k, "Dissolve");

			if(this->validateTransition(k, transitionList))
				transitionList->push_back(*transition);
		}
	}
}

/************************************************************************
* Função que calcula a segunda derivada através do histograma do ritmo 
* visual
*************************************************************************
* param (E): vetor de entrada
*************************************************************************
* return : float -> valor da segunda derivada do histograma do ritmo 
* visual
*************************************************************************
* Histórico:
* 18/08/08 - Ivan Shiguenori Machida
* Criação.
************************************************************************/
int Dissolve::calcFirstDerivative(double firstFrame, double secondFrame, double thresholdMax, double thresholdMin)
{
	if(((secondFrame-firstFrame) < thresholdMax) && ((secondFrame-firstFrame) > thresholdMin))
		return(1);
	else
		return(0);
}

double* Dissolve::calcDerivative(double *array, int size_i)
{
	// Para calcular a derivada no eixo y , calculamos para cada ponto fy:
	// dy = ((f(y+1) - f(y-1)) / 2)

	//double dy;
	int i = 0;
	double *array_dy;

//	Log::writeLog("%s :: array[%x] size_i[%d]", __FUNCTION__, array, size_i);

	array_dy = (double*)malloc(sizeof(double)*size_i);

	// Calculo para a posicao 0
	array_dy[i] = ( 0 - array[i] ) / 2;
//	Log::writeLog("%s :: array_y[%d] = %lf array_dy[%d] = %lf", __FUNCTION__, i, array[i], i, array_dy[i]);

	for ( i = 1 ; i < size_i ; i++ )
	{
		array_dy[i] = (( array[i+1] - array[i-1] ) / 2);
//		Log::writeLog("%s :: array_y[%d] = %lf array_dy[%d] = %lf", __FUNCTION__, i, array[i], i, array_dy[i]);
	}

	return array_dy;
}

double* Dissolve::calcVariance(Video* vdo)
{
	Frame* frame = new Frame(); 
	Frame* frameGray = 0;
	Frame *frameDiagonal = 0;
	Color *color = new Color();

	double totalFrames = vdo->getFramesTotal();

	int posic = 0;

	double* variance;

	variance = (double*)malloc(sizeof(double)*cvRound(totalFrames));

	memset(variance, '\0',     sizeof(double)*cvRound(totalFrames));

	// Pego o primeiro frame
	frame = vdo->getNextFrame();	

	while(frame != NULL)
	{

		if (posic >= totalFrames)
		{
			Log::writeLog("%s :: BOOOOOOM!", __FUNCTION__);
		}

		// Converto o frame para escala de cinza.
		frameGray = color->convert2Gray(frame);

		// Pega a diagonal
		frameDiagonal = frameGray->getDiagonal();

		// Guardo a media do valor de luminancia da diagonal.
		variance[posic] = frameGray->pointVariance();
	
		Log::writeLog("variance[%lf] posic[%d]", variance[posic], posic);

		/**
		 * Deleto os objetos criados anteriormente para desalocamento de
		 * memoria.
		**/
		delete frameDiagonal;
		delete frameGray;
		delete frame;
		
		// Pego o proximo
		frame = vdo->getNextFrame();

		posic++;

	}

	delete color;
	// Retorno o array com os valores do RVH

	return (variance);
}
