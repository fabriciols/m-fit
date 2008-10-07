#include "cv.h"
#include "highgui.h"
#include <stdexcept>
#include <vector>
#include <QImage>

#include "../include/Time.h"
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
* Hist�rico:
* 18/08/08 - Ivan Shiguenori Machida
* Cria��o.
************************************************************************/
Dissolve::Dissolve()
{
}

/************************************************************************
* Construtor que somente inicializa as variavies de controle com nulo 
*************************************************************************
* param (E): Video *vdo - guarda o video corrente para calculos de
* detec��o
*************************************************************************
* Hist�rico:
* 18/08/08 - Ivan Shiguenori Machida
* Cria��o.
************************************************************************/
Dissolve::Dissolve(Video *vdo)
{
	this->video = vdo;
}

/************************************************************************
* Fun��o que detecta as transi��es do tipo Dissolve
*************************************************************************
* param (E): Video* vdo -> video � detectar as transi��es
* param (S): Transition* transitions -> Posic�o corrente da lista de transi��es
*************************************************************************
* return : Nenhum
*************************************************************************
* Hist�rico:
* 19/08/08 - Ivan Shiguenori Machida
* Cria��o.
************************************************************************/
void Dissolve::detectTransitions(Video* vdo, std::vector<Transition>* transitionList)
{
	Transition *transition;
	Frame* visual = new Frame();
	double media=0, variancia, *array_dvrh;
	int len_i, i, j, k;

	Color *color = new Color();
	Frame* frameGray = 0;
	double *pontos, *pontos_anterior;
	int tamanho_pontos, *valor_pontos, *detecta;

	double *calculo_pontos=0;

	char pri[4],seg[4],ter[4],qua[4];

	len_i = cvRound(vdo->getFramesTotal());

	visual = vdo->getCurrentFrame();

	tamanho_pontos = (visual->getWidth()*visual->getHeight());

	pontos = (double *)malloc(sizeof(double)*tamanho_pontos);
	pontos_anterior = (double *)malloc(sizeof(double)*tamanho_pontos);
	valor_pontos = (int *)malloc(sizeof(int)*len_i);
	calculo_pontos = (double *)malloc(sizeof(double)*len_i);
	array_dvrh = (double *) malloc(sizeof(double)*len_i);
	detecta = (int *) malloc(sizeof(int)*len_i);

	memset(pontos,0,tamanho_pontos);
	memset(pontos_anterior,0,tamanho_pontos);

	memset(detecta,0,sizeof(detecta));

	for(k=0;k<len_i;k++)
	{
		visual = vdo->getNextFrame();
		frameGray = color->convert2Gray(visual);

		media = array_dvrh[k] = frameGray->mediaBin();

		j=0;
		for(i=0;i<tamanho_pontos;i++)
		{
			variancia += (pontos[i] - media) * (pontos[i] - media);
		}

		variancia = variancia / (tamanho_pontos-1);

		valor_pontos[k] = j;

		variancia=0;
	}

	for(i=0;i<len_i;i++)
	{
		sprintf(pri,"%03.0lf",array_dvrh[i]);
		sprintf(seg,"%03.0lf",array_dvrh[i+1]);
		sprintf(ter,"%03.0lf",array_dvrh[i+2]);
		sprintf(qua,"%03.0lf",array_dvrh[i+3]);

		if(((atoi(seg)-atoi(pri))==1) && ((atoi(ter)-atoi(seg))==1) && ((atoi(qua)-atoi(ter))==1))
			detecta[i]=1;
		else
			detecta[i]=0;
	}

	for(i=0;i<len_i;i++)
	{
		if(detecta[i]==1 && detecta[i+1]==1 && detecta[i+2]==1)
		{
			transition = new Transition(TRANSITION_DISSOLVE, i+1, "Dissolve");
			transitionList->push_back(*transition);
			detecta[i]=1;
		}
		else
			detecta[i]=0;
	}

	delete detecta;
	delete valor_pontos;
	delete array_dvrh;
	delete pontos;
	delete pontos_anterior;
	delete calculo_pontos;
}

/************************************************************************
* Fun��o que calcula a variancia atrav�s do histograma do ritmo visual
*************************************************************************
* param (E): nenhum
*************************************************************************
* return : float -> valor da vari�ncia do histograma do ritmo visual
*************************************************************************
* Hist�rico:
* 19/08/08 - Ivan Shiguenori Machida
* Efetuando os calculos para variancia e desvio padrao.
* 17/08/08 - Ivan Shiguenori Machida
* Cria��o.
************************************************************************/
float Dissolve::calcVariance()
{
	return(0);
}

/************************************************************************
* Fun��o que calcula a segunda derivada atrav�s do histograma do ritmo 
* visual
*************************************************************************
* param (E): nenhum
*************************************************************************
* return : float -> valor da segunda derivada do histograma do ritmo 
* visual
*************************************************************************
* Hist�rico:
* 18/08/08 - Ivan Shiguenori machida
* Cria��o.
************************************************************************/
float Dissolve::calcSecondDerivative()
{
	return(0);	
}

/************************************************************************
* Fun��o que calcula a raz�o entre variancia e a segunda derivada gerada 
* atrav�s do histograma do ritmo visual
*************************************************************************
* param (E): nenhum
*************************************************************************
* return : float -> valor raz�o entre variancia e a segunda derivada 
* gerada atrav�s do histograma do ritmo visual
*************************************************************************
* Hist�rico:
* 18/08/08 - Ivan Shiguenori machida
* Cria��o.
************************************************************************/
float Dissolve::calcRatioVarianceVRH()
{
	return(0);
}
