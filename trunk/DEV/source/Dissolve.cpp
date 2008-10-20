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

/************************************************************************
* Construtor que somente inicializa as variavies de controle com nulo 
*************************************************************************
* param (E): nenhum
*************************************************************************
* Histórico:
* 18/08/08 - Ivan Shiguenori Machida
* Criação.
************************************************************************/
Dissolve::Dissolve()
{
}

/************************************************************************
* Construtor que somente inicializa as variavies de controle com nulo 
*************************************************************************
* param (E): Video *vdo - guarda o video corrente para calculos de
* detecção
*************************************************************************
* Histórico:
* 18/08/08 - Ivan Shiguenori Machida
* Criação.
************************************************************************/
Dissolve::Dissolve(Video *vdo)
{
	this->video = vdo;
}

/************************************************************************
* Função que detecta as transições do tipo Dissolve
*************************************************************************
* param (E): Video* vdo -> video à detectar as transições
* param (S): Transition* transitions -> Posicão corrente da lista de transições
*************************************************************************
* return : Nenhum
*************************************************************************
* Histórico:
* 16/10/08 - Mauricio Hirota
* Criação.
************************************************************************/
void Dissolve::detectTransitions(Video* vdo, std::vector<Transition>* transitionList)
{
	double *array_vrh;
	int len_i, k, threshold=0, threshold_fade = 0;

	Frame* visual = new Frame();
	Frame* frameGray = 0;

	int *detet;

	len_i = cvRound(vdo->getFramesTotal());

	visual = vdo->getCurrentFrame();

	detet = (int *) malloc(sizeof(int)*len_i);
	array_vrh = (double *) malloc(sizeof(double)*len_i);

	memset(detet,0,len_i);

	//Coleta o ritmo visual dos frames
	for(k=0;k<len_i;k++)
	{
		Color *color = new Color();
		Frame* visual = new Frame();
		visual = vdo->getNextFrame();
		frameGray = color->convert2Gray(visual);

		array_vrh[k] = frameGray->mediaBin();

		delete color;
		delete frameGray;
		delete visual;
	}

	threshold = 4;
	threshold_fade = 5;

	//Verifica ponto de dissolve
	for(k=0;k<len_i;k++)
	{
		if((array_vrh[k]<array_vrh[k+1]) &&
		   (array_vrh[k+1]<array_vrh[k+2]) &&
		   (array_vrh[k+2]<array_vrh[k+3]) &&
		   (array_vrh[k+3]<array_vrh[k+4]) &&
		   (array_vrh[k+4]<array_vrh[k+5]) &&
		   (array_vrh[k+5]<array_vrh[k+6]) &&
		   (array_vrh[k+6]<array_vrh[k+7]))
		{
			//aplica 1. derivada
			if(((array_vrh[k+1]-array_vrh[k])<threshold) &&
			  ((array_vrh[k+2]-array_vrh[k+1])<threshold) &&
			  ((array_vrh[k+3]-array_vrh[k+2])<threshold) &&
			  ((array_vrh[k+4]-array_vrh[k+3])<threshold) &&
			  ((array_vrh[k+5]-array_vrh[k+4])<threshold) &&
			  ((array_vrh[k+6]-array_vrh[k+5])<threshold) &&
			  ((array_vrh[k+7]-array_vrh[k+6])<threshold))
			{
				detet[k]=1;
				k=k+6;
			}
		}

		if((array_vrh[k]>array_vrh[k+1]) &&
		   (array_vrh[k+1]>array_vrh[k+2]) &&
		   (array_vrh[k+2]>array_vrh[k+3]) &&
		   (array_vrh[k+3]>array_vrh[k+4]) &&
		   (array_vrh[k+4]>array_vrh[k+5]) &&
		   (array_vrh[k+5]>array_vrh[k+6]) &&
		   (array_vrh[k+6]>array_vrh[k+7]))
		{
			//aplica 1. derivada
			if(((array_vrh[k]-array_vrh[k+1])<threshold) &&
			  ((array_vrh[k+1]-array_vrh[k+2])<threshold) &&
			  ((array_vrh[k+2]-array_vrh[k+3])<threshold) &&
			  ((array_vrh[k+3]-array_vrh[k+4])<threshold) &&
			  ((array_vrh[k+4]-array_vrh[k+5])<threshold) &&
			  ((array_vrh[k+5]-array_vrh[k+6])<threshold) &&
			  ((array_vrh[k+6]-array_vrh[k+7])<threshold))
			{
				detet[k]=1;
				k=k+6;
			}
		}
	}

	//Retirando redundancias
	for(k=0;k<len_i;k++)
	{
		if(detet[k])
		{
			while(detet[k+7]==1)
			{
				detet[k+7]=0;
				k=k+7;
			}
		}
	}

	//Verifica se é um fade
	for(k=0;k<len_i;k++)
	{
		if(detet[k]==1)
		{
			if((array_vrh[k]<threshold_fade) ||
			   (array_vrh[k-1]<threshold_fade) ||
			   (array_vrh[k-2]<threshold_fade) ||
			   (array_vrh[k-3]<threshold_fade) ||
			   (array_vrh[k-4]<threshold_fade) ||
			   (array_vrh[k-5]<threshold_fade) ||
			   (array_vrh[k-6]<threshold_fade) ||
			   (array_vrh[k+1]<threshold_fade) ||
			   (array_vrh[k+2]<threshold_fade) ||
			   (array_vrh[k+3]<threshold_fade) ||
			   (array_vrh[k+4]<threshold_fade) ||
			   (array_vrh[k+5]<threshold_fade) ||
			   (array_vrh[k+6]<threshold_fade))
			{
				detet[k]=0;
			}
		}
	}

	//Transportando para matriz de detecção
	for(k=0;k<len_i;k++)
	{
		if(detet[k]==1)
		{
			Transition *transition = new Transition();
			transition = new Transition(TRANSITION_DISSOLVE, k, "Dissolve");
			transitionList->push_back(*transition);
		}
	}
}

/************************************************************************
* Função que calcula a variancia através do histograma do ritmo visual
*************************************************************************
* param (E): vetor de média
*************************************************************************
* return : float -> valor da variância do histograma do ritmo visual
*************************************************************************
* Histórico:
* 19/08/08 - Ivan Shiguenori Machida
* Efetuando os calculos para variancia e desvio padrao.
* 17/08/08 - Ivan Shiguenori Machida
* Criação.
************************************************************************/
double Dissolve::calcVariance(double *arrayM, int size_i )
{
	int i;
	double mean = 0;
	double variance=0;
   double soma=0;
	// faz a média
	for (i=1; i<=size_i; i++)
	{
		soma=soma + arrayM[i];
	}
	mean = soma / size_i;
	
	// faz variancia
	for (i=1; i<=size_i; i++)
	{
		variance = variance + ((arrayM[i] - mean ) * (arrayM[i] - mean));
	}
	variance = variance / (size_i-1);
	return(variance);
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
* 18/08/08 - Ivan Shiguenori machida
* Criação.
************************************************************************/
double Dissolve::calcSecondDerivative(double *array, int size)
{
	double *derivative2 = (double *)malloc(sizeof(double)*size);
	double *derivative1 = (double *)malloc(sizeof(double)*size);
	int i;
	// Calcula segunda derivada, se for primeiro ou ultimo frame, derivada  = 0
	for (i=1; i<=size; i++)
	{
		if (i == 1 || i == size)
			derivative1[i] = 0;
		else
			derivative1[i] = (array[i+1] + array[i-1])/2;
	}
	for (i=1; i<=size; i++)
	{
		if (i == 1 || i == size)
			derivative2[i] = 0;
		else
	
		derivative2[i] = (derivative1[i+1] + derivative1[i-1])/2;
	}
	
	return(*derivative2);	
}

/************************************************************************
* Função que calcula a razão entre variancia e a segunda derivada gerada 
* através do histograma do ritmo visual
*************************************************************************
* param (E): variancia e segunda derivada
*************************************************************************
* return : float -> valor razão entre variancia e a segunda derivada 
* gerada através do histograma do ritmo visual
*************************************************************************
* Histórico:
* 18/08/08 - Ivan Shiguenori machida
* Criação.
************************************************************************/
double Dissolve::calcRatioVarianceVRH(double variance, double secondDerivative)
{
	if (secondDerivative!=0)
		return(variance / secondDerivative);
	else
		return 0;
}
