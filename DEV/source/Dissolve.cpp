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
* Função que calcula a variancia através do ritmo visual
*************************************************************************
* param (E): double array -> array a ser derivado no eixo y
*************************************************************************
* return : float -> valor da variância
*************************************************************************
* Histórico:
* 17/08/08 - Ivan Shiguenori machida
* Criação.
************************************************************************/
float Dissolve::calcVariance()
{
	VisualRythim *vr = new VisualRythim();
	Video *vdo = this->video;
	Frame* visual = new Frame();
	double *array_vrh;
	int len_i, i;

	array_vrh = vr->createVRH(vdo);
//	visual = vr->createVR(vdo);

	len_i = cvRound(vdo->getFramesTotal());

	printf("len_i[%d]\n", len_i);

	for(i=0;i<len_i;i++)
	{
		printf("array_vrh[%lf]\n", array_vrh[i]);
	}
}

Dissolve::Dissolve(char *video)
{
}

Dissolve::Dissolve()
{
}

Dissolve::Dissolve(Video *vdo)
{
	this->video = vdo;
}
