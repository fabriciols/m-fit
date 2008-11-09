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
* 16/10/08 - Mauricio Hirota / Ivan Shiguenori Machida
* Criação.
* 08/11/08 - Ivan Shiguenori Machida
* Utilizacao da funcao calcFirstDerivative
************************************************************************/
void Dissolve::detectTransitions(Video* vdo, std::vector<Transition>* transitionList)
{
	double *array_vrh;
	int videoSize, k, i, threshold=0, threshold_fade = 0;
	Frame* frameGray = 0;

	int *detect;
	int *orientation;

	videoSize = cvRound(vdo->getFramesTotal());

	detect = (int *) malloc(sizeof(int)*videoSize);
	orientation = (int *) malloc(sizeof(int)*videoSize);
	array_vrh = (double *) malloc(sizeof(double)*videoSize);

	memset(detect,0,videoSize);
	memset(orientation,0,videoSize);

	//Coleta o ritmo visual dos frames
	for(k=0;k<videoSize;k++)
	{
		Color *color = new Color();
		Frame* visual = new Frame();
		visual = vdo->getNextFrame();
		frameGray = color->convert2Gray(visual);

		this->detectBorder(frameGray);
		array_vrh[k] = frameGray->mediaBin();

		delete color;
		delete frameGray;
		delete visual;
	}

	threshold = 4;

	//Verifica ponto de dissolve
	for(k=0;k<videoSize;k++)
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
			if((calcFirstDerivative(array_vrh[k],array_vrh[k+1])<threshold) &&
			  (calcFirstDerivative(array_vrh[k+1],array_vrh[k+2])<threshold) &&
			  (calcFirstDerivative(array_vrh[k+2],array_vrh[k+3])<threshold) &&
			  (calcFirstDerivative(array_vrh[k+3],array_vrh[k+4])<threshold) &&
			  (calcFirstDerivative(array_vrh[k+4],array_vrh[k+5])<threshold) &&
			  (calcFirstDerivative(array_vrh[k+5],array_vrh[k+6])<threshold) &&
			  (calcFirstDerivative(array_vrh[k+6],array_vrh[k+7])<threshold))
			{
				orientation[k]=RIGHT;
				detect[k]=1;
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
			if((calcFirstDerivative(array_vrh[k+1],array_vrh[k])<threshold) &&
			  (calcFirstDerivative(array_vrh[k+2],array_vrh[k+1])<threshold) &&
			  (calcFirstDerivative(array_vrh[k+3],array_vrh[k+2])<threshold) &&
			  (calcFirstDerivative(array_vrh[k+4],array_vrh[k+3])<threshold) &&
			  (calcFirstDerivative(array_vrh[k+5],array_vrh[k+4])<threshold) &&
			  (calcFirstDerivative(array_vrh[k+6],array_vrh[k+5])<threshold) &&
			  (calcFirstDerivative(array_vrh[k+7],array_vrh[k+6])<threshold))
			{
				orientation[k]=LEFT;
				detect[k]=1;
				k=k+6;
			}
		}
	}

	//Retirando redundancias
	for(k=0;k<videoSize;k++)
	{
		if(detect[k])
		{
			while(detect[k+7]==1)
			{
				detect[k+7]=0;
				k=k+7;
			}
		}
	}

	//Verifica se é um fade
	i=0;
	threshold_fade = 5;

	for(k=0;k<videoSize;k++)
	{
		if(detect[k]==1)
		{
			i=k;

			if(orientation[k]==RIGHT)
			{
				while((array_vrh[i]<array_vrh[i+1]) && (i<videoSize))
				{
//					Log::writeLog("RIGHT %d - %lf", k, array_vrh[i]);
					if(array_vrh[i]<threshold_fade)
					{
						detect[k]=0;
					}
					i++;
				}
			}
			else if(orientation[k]==LEFT)
			{
				while((array_vrh[i]>array_vrh[i+1]) && (i<videoSize))
				{
//					Log::writeLog("LEFT %d - %lf", k, array_vrh[i]);
					if(array_vrh[i]<threshold_fade)
					{
						detect[k]=0;
					}
					i++;
				}
			}
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
double Dissolve::calcFirstDerivative(double firstFrame, double secondFrame)
{
	return(secondFrame-firstFrame);
}

/************************************************************************
* Detecta a borda do frame usando filtro Canny
*************************************************************************
* param (E): vetor de entrada
*************************************************************************
* return : void
*************************************************************************
* Histórico:
* 08/11/08 - Ivan Shiguenori Machida
* Criação.
************************************************************************/
void Dissolve::detectBorder(Frame *frameMap)
{
	Filters* border = new Filters();
	
	border->Canny(frameMap, 240, 255, 3);

	delete border;
}
