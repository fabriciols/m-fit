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
* Fun��o que detecta as transi��es do tipo Dissolve
*************************************************************************
* param (E): Video* vdo -> video � detectar as transi��es
* param (S): Transition* transitions -> Posic�o corrente da lista de transi��es
*************************************************************************
* return : Nenhum
*************************************************************************
* Hist�rico:
* 18/11/08 - Thiago Mizutani
* Recria��o da fun��o
* 08/11/08 - Ivan Shiguenori Machida
* Utilizacao da funcao calcFirstDerivative
* 16/10/08 - Mauricio Hirota / Ivan Shiguenori Machida
* Cria��o.
************************************************************************/
void Dissolve::detectTransitions(Video* vdo, std::vector<Transition>* transitionList)
{
	double* visualRythim; // Array para receber o RVH.
	double* variance; // Variancia das diagonais dos frames do v�deo.
	double* firstDerVariance; // Primeira derivada da variancia
	double* secondDerVariance; // Segunda derivada da variancia
	double* firstDerLuminance; // Primeira derivada da media da luminancia
	double* ratio; // Diferenca entre a 2a derivada da variancia e 1a derivada da media da luminancia

	double difference;

	int threshold = 2; // Limiar para valida��o do dissolve (referencia de artigo).

	int videoSize = 0; // Tamanho do v�deo (em frames).
	int i = 0;
	int j = 0;
	int countNext = 0; // Contador para os pr�ximos frames
	int countPrevious = 0; // Contador para os frames anteriores.

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

	// Retirando partes indesej�veis do v�deo.
	vdo->removeWide();
	vdo->removeBorder();

	// obtenho o ritmo visual. Com isso j� tenho a m�dia da luminancia dos frames.
	visualRythim = vrh->createVRH(vdo);

	// Depois de criar o RVH tenho que voltar o ponteiro para o primeiro frame.
	vdo->seekFrame(0);

	currentFrame = vdo->getCurrentFrame();

	Log::writeLog("%s :: Calculando variancia", __FUNCTION__);

	// Calculo a variancia da diagonal de cada frame do v�deo.
	for(i=0; i<videoSize; i++)
	{
		variance[i] = currentFrame->calculateVariance(visualRythim[i]);

		delete currentFrame;
		
		currentFrame = vdo->getNextFrame();

		if(!currentFrame)
			break;
	}

	// Calculando as derivadas (todas ao mesmo tempo).
	firstDerLuminance[0] = (visualRythim[0]*-1)/2;
	firstDerVariance[0] = (variance[0]*-1)/2;

	for(i=1;i<videoSize;i++)
	{
		firstDerLuminance[i] = (visualRythim[i+1] - visualRythim[i-1])/2.0;
		firstDerVariance[i] = (variance[i+1] - variance[i-1])/2.0;
		if((i-1) == 0)
			secondDerVariance[0] = (firstDerVariance[0]*-1)/2.0;
		else
			secondDerVariance[i-1] = (firstDerVariance[i] - firstDerVariance[i-2])/2.0;
	}

	// Como esta derivada come�ou 1 posi��o depois, devo preencher a ultima posi��o aqui.
	secondDerVariance[i] = (firstDerVariance[i+1] - firstDerVariance[i-1])/2.0;

	Log::writeLog("%s :: second = %lf", __FUNCTION__, secondDerVariance[i]);

//	dissolvePoints = validateDissolve(ratio, visualRythim, threshold, videoSize);

	for( i=0; i<videoSize; i++ )		
	{
		// Calculando a raz�o 2a Derivada da variancia / 1a Derivada da media da luminancia
		if(firstDerLuminance[i] != 0.0 && secondDerVariance[i] != 0.0)
		{
			ratio[i] = secondDerVariance[i]/firstDerLuminance[i];
			ratio[i+1] = secondDerVariance[i+1]/firstDerLuminance[i+1];
		}
		else 
		{
			ratio[i] = 0.0;
			ratio[i+1] = 0.0;
		}

		// Diferen�a entre a raz�o de um frame e o pr�ximo, de forma que o valor deve estar em m�dulo.
		difference = fabs(fabs(ratio[i+1]) - fabs(ratio[i]));

		Log::writeLog("%s :: secondDerVariance[%d] = %lf", __FUNCTION__, i, secondDerVariance[i]);
		Log::writeLog("%s :: firstDerLuminance[%d] = %lf", __FUNCTION__, i, firstDerLuminance[i]);
		Log::writeLog("%s :: ratio[%d] = %lf", __FUNCTION__, i, ratio[i]);
		Log::writeLog("%s :: nextRatio[%d] = %lf", __FUNCTION__, i+1, ratio[i+1]);
		Log::writeLog("%s :: difference = %lf", __FUNCTION__, difference);

		if(difference <= threshold)
		{
			// Se os pr�ximos 10 frames estiverem mto escuros, significa que � um fade-out
			for(j=i; j<i+10 && j<videoSize; j++)		
			{
				
				Log::writeLog("%s :: visualRythim[%d] = %lf, i = %d", __FUNCTION__, j, visualRythim[j], i);

				if(visualRythim[j] < 10)
				{
					countNext++;
				}
			}

			// Se os 10 frames anteriores estiverem mto escuros, significa que � um fade-in
			for(j=i; j>i-10 && j>0; j--)
			{
				if(visualRythim[j] < 10)
				{
					countPrevious++;
				}
			}	Log::writeLog("%s :: countNext[%d] = %d", __FUNCTION__, i, countNext);
			
			Log::writeLog("%s :: countNext[%d] = %d", __FUNCTION__, i, countNext);
			Log::writeLog("%s :: countPrevious[%d] = %d", __FUNCTION__, i, countPrevious);

			if(countNext < 3 && countPrevious < 3)
			{
				Transition* newTransition = new Transition(TRANSITION_DISSOLVE,i,"Dissolve");
				transitionList->push_back(*newTransition);
			}

			countNext = 0;
			countPrevious = 0;
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

	// C�lculo para a primeira posi��o.
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
			// Verifico os pr�ximos 10 frames. se n�o for 0 eu mato.

				dissolvePoints[i] = 1;
		}
	}

	return (dissolvePoints);
}
