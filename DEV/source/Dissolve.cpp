#include "cv.h"
#include "highgui.h"
#include <stdexcept>

#include "../include/Time.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"

#include "../include/Video.h"
#include "../include/Transition.h"
#include "../include/DetectTransitions.h"
#include "../include/VisualRythim.h"

#include "../include/Dissolve.h"

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
* Função que calcula a variancia através do histograma do ritmo visual
*************************************************************************
* param (E): nenhum
*************************************************************************
* return : float -> valor da variância do histograma do ritmo visual
*************************************************************************
* Histórico:
* 19/08/08 - Ivan Shiguenori Machida
* Efetuando os calculos para variancia e desvio padrao.
* 17/08/08 - Ivan Shiguenori Machida
* Criação.
************************************************************************/
float Dissolve::calcVariance()
{
	VisualRythim *vr = new VisualRythim();
	Video *vdo = this->video;
	Frame* visual = new Frame();
	Frame* frameVRH;
	double *array_vrh, media=0, variancia, variancia1[255], desviopadrao=0, 
		   array_dvrh[255], array_ddvrh[255], ratio[255], ddvariancia[255];
	int len_i, i, j;

	array_vrh = vr->createVRH(vdo);
//	visual = vr->createVR(vdo);

	len_i = cvRound(vdo->getFramesTotal());

	printf("len_i[%d]\n", len_i);

	memset(array_dvrh, 0, sizeof(array_dvrh));

	i=0;
	media += array_vrh[i];
	array_dvrh[i] = array_vrh[i+1] - array_vrh[i] +50;
	printf("vrh[%lf]\n", array_vrh[i]);

	for(i=1;i<len_i-1;i++)
	{
		media += array_vrh[i];
		array_dvrh[i] = array_vrh[i+1] - array_vrh[i] +50;
		array_ddvrh[i] = (array_vrh[i+1] + array_vrh[i-1]) - (2*array_vrh[i]);
	}

	i=len_i-1;
	media += array_vrh[i];
	array_dvrh[i] = array_vrh[i+1] - array_vrh[i] +50;
	printf("vrh[%lf]\n", array_vrh[i]);

	media = media / len_i;

	printf("media[%lf]\n", media);

	for(i=0;i<len_i;i++)
	{
		variancia += (array_vrh[i] - media) * (array_vrh[i] - media);
		variancia1[i] = ((array_vrh[i] - media) * (array_vrh[i] - media))/(len_i-1);
	}

	printf("A-variancia[%lf]\n", variancia);

	variancia = variancia / (len_i-1);

	printf("D-variancia[%lf]\n", variancia);

	desviopadrao = sqrt(variancia);

	printf("desviopadrao[%lf]\n", desviopadrao);

	for(i=1;i<len_i-1;i++)
	{
		ddvariancia[i] = (variancia1[i+1] + variancia1[i-1]) - (2*variancia1[i]);
		ratio[i] = array_dvrh[i] / ddvariancia[i];
		printf("vrh[%lf] - dvrh[%lf] - ddvrh[%lf] - ratio[%lf]\n", array_vrh[i], array_dvrh[i], array_ddvrh[i], ratio[i]);
	}

	frameVRH = new Frame(array_vrh, len_i, 256);
	cvShowImage(vdo->getName(), frameVRH->data);

	sprintf(this->tmp1, "nova1_%s.jpg", vdo->getName());
	frameVRH->write(this->tmp1);

	cvWaitKey(0);
/*
	frameVRH = new Frame(ratio, len_i, 256);
	cvShowImage(vdo->getName(), frameVRH->data);

	cvWaitKey(0);

	frameVRH = new Frame(array_dvrh, len_i, 256);
	cvShowImage(vdo->getName(), frameVRH->data);

	cvWaitKey(0);
*/
}

/************************************************************************
* Função que calcula a segunda derivada através do histograma do ritmo 
* visual
*************************************************************************
* param (E): nenhum
*************************************************************************
* return : float -> valor da segunda derivada do histograma do ritmo 
* visual
*************************************************************************
* Histórico:
* 18/08/08 - Ivan Shiguenori machida
* Criação.
************************************************************************/
float Dissolve::calcSecondDerivative()
{
	
}

/************************************************************************
* Função que calcula a razão entre variancia e a segunda derivada gerada 
* através do histograma do ritmo visual
*************************************************************************
* param (E): nenhum
*************************************************************************
* return : float -> valor razão entre variancia e a segunda derivada 
* gerada através do histograma do ritmo visual
*************************************************************************
* Histórico:
* 18/08/08 - Ivan Shiguenori machida
* Criação.
************************************************************************/
float Dissolve::calcRatioVarianceVRH()
{

}
