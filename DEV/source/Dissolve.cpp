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
* 18/11/08 - Thiago Mizutani
* Recriação da função
* 08/11/08 - Ivan Shiguenori Machida
* Utilizacao da funcao calcFirstDerivative
* 16/10/08 - Mauricio Hirota / Ivan Shiguenori Machida
* Criação.
************************************************************************/
void Dissolve::detectTransitions(Video* vdo, std::vector<Transition>* transitionList)
{
	double* visualRythim;
	double* variance; // Variancia das diagonais dos frames do vídeo.
	double* firstDerVariance; // Primeira derivada da variancia
	double* secondDerVariance; // Segunda derivada da variancia
	double* firstDerLuminance; // Primeira derivada da media da luminancia
	double* ratio; // Diferenca entre a 2a derivada da variancia e 1a derivada da media da luminancia

	double difference;

	int threshold = 2; // Limiar para validação do dissolve (referencia de artigo).
	int* dissolvePoints; // Pontos de ocorrencia do dissolve.

	int videoSize = 0; // Tamanho do vídeo (em frames).
	int i = 0;
	VisualRythim* vrh = new VisualRythim();
	Frame* currentFrame;

	videoSize = cvRound(vdo->getFramesTotal());

	variance = (double*)malloc(sizeof(double)*videoSize);
	memset(variance,'\0',sizeof(variance));

	ratio = (double*)malloc(sizeof(double)*videoSize);
	memset(ratio,'\0',sizeof(ratio));

	firstDerVariance = (double*)malloc(sizeof(double)*videoSize);
	memset(firstDerVariance,'\0',sizeof(firstDerVariance));

	secondDerVariance = (double*)malloc(sizeof(double)*videoSize);
	memset(secondDerVariance,'\0',sizeof(secondDerVariance));
	
	firstDerLuminance = (double*)malloc(sizeof(double)*videoSize);
	memset(firstDerLuminance,'\0',sizeof(firstDerLuminance));

	if(vdo->getCurrentPosition() != 0)
		vdo->seekFrame(0);

	// Retirando partes indesejáveis do vídeo.
	vdo->removeWide();
	vdo->removeBorder();

	// obtenho o ritmo visual. Com isso já tenho a média da luminancia dos frames.
	visualRythim = vrh->createVRH(vdo);

	// Depois de criar o RVH tenho que voltar o ponteiro para o primeiro frame.
	vdo->seekFrame(0);

	currentFrame = vdo->getCurrentFrame();

	Log::writeLog("%s :: Calculando variancia", __FUNCTION__);

	// Calculo a variancia da diagonal de cada frame do vídeo.
	for(i=0; i<videoSize; i++)
	{
		variance[i] = currentFrame->calculateVariance(visualRythim[i]);

		delete currentFrame;
		
		currentFrame = vdo->getNextFrame();

		if(!currentFrame)
			break;
	}

	Log::writeLog("%s :: derivadas", __FUNCTION__);
	Log::writeLog("%s :: visual = %lf", __FUNCTION__, visualRythim[0]);
	Log::writeLog("%s :: variance = %lf", __FUNCTION__, variance[0]);

	firstDerLuminance[0] = (visualRythim[0]*-1)/2;
	firstDerVariance[0] = (variance[0]*-1)/2;

	Log::writeLog("%s :: 1a der lum = %lf , 1a der var = %lf", __FUNCTION__,firstDerLuminance[0],firstDerVariance[0]);

	for(i=1;i<videoSize;i++)
	{
		Log::writeLog("%s :: i = %d", __FUNCTION__, i);
		firstDerLuminance[i] = (visualRythim[i+1] - visualRythim[i-1])/2.0;
		firstDerVariance[i] = (variance[i+1] - variance[i-1])/2.0;
		if((i-1) == 0)
			secondDerVariance[0] = (firstDerVariance[0]*-1)/2.0;
		else
			secondDerVariance[i-1] = (firstDerVariance[i] - firstDerVariance[i-2])/2.0;
	}

	Log::writeLog("%s :: ultima derivada", __FUNCTION__);

	secondDerVariance[i] = (firstDerVariance[i+1] - firstDerVariance[i-1])/2.0;

//	dissolvePoints = validateDissolve(ratio, visualRythim, threshold, videoSize);

	for( i=0; i<videoSize; i++ )		
	{
		ratio[i] = secondDerVariance[i]/firstDerLuminance[i];
		ratio[i+1] = secondDerVariance[i+1]/firstDerLuminance[i+1];

		difference = fabs(fabs(ratio[i+1]) - fabs(ratio[i]));

		if(difference <= threshold)
		{
			Transition* newTransition = new Transition(TRANSITION_DISSOLVE,i,"Dissolve");
			transitionList->push_back(*newTransition);
		}
	}
	
	delete currentFrame;
	delete vrh;
}

double* Dissolve::calculateDerivative(double* curve, int size_i)
{
	int i = 0;
	double* derivative;

	derivative = (double*)malloc(sizeof(double)*size_i);

	// Cálculo para a primeira posição.
	derivative[i] = (0 - curve[i]) / 2;


	for( i=1; i<size_i; i++ )
	{
		derivative[i] = ( (curve[i+1] - curve[i-1])/2 );
	}

	return (derivative);
}

int* Dissolve::validateDissolve(double* ratio, double* vrh, int threshold, int size_i)
{
	int i = 0;

	double difference = 0;
	int* dissolvePoints;

	dissolvePoints = (int*)malloc(sizeof(int)*size_i);

	memset(dissolvePoints,'\0',sizeof(dissolvePoints));

	for( i=1; i<size_i; i++)
	{
		difference = fabs(fabs(ratio[i]) - fabs(ratio[i-1]));

		if(difference <= (double)threshold)
		{
			// Verifico os próximos 10 frames. se não for 0 eu mato.

				dissolvePoints[i] = 1;
		}
	}

	return (dissolvePoints);
}
