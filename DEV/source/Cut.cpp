#include "cv.h"
#include "highgui.h"

#include "../include/Time.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/Transition.h"
#include "../include/VisualRythim.h"
#include "../include/DetectTransitions.h"
#include "../include/Cut.h"
#include "../include/Filters.h"

/************************************************************************
* Fun��o que faz a detec��o das transi��es do tipo corte.
*************************************************************************
* param (E): int type => tipo de detec��o de transi��o a ser feita.
* 								 0 -> todas
* 								 1 -> s� cortes
*				 Video* vdo => v�deo onde ser� feita a detec��o
*************************************************************************
* return : Lista de transi��es.
*************************************************************************
* Hist�rico:
* 13/08/08 - Thiago Mizutani
* Chamadas das fun��es que completam o processo de detec��o de cortes.
* 06/07/08 - Thiago Mizutani
* Ordena��o das chamadas de fun��o.
* 04/07/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

Transition* Cut::detectTransitions(Video* vdo, int type)
{ 
   Frame* frameAux = 0;
	Filters* filters = 0;

	// Se a posicao do video n�o for a inicial, aponto para o primeiro frame.
	if (vdo->getCurrentPosition() != 0)
		vdo->seekFrame(0);

	VisualRythim *vr = 0;

	Frame *visualRythim = new Frame(vr->createVR(vdo));

	frameAux = filters->lowPass(visualRythim, 5);
	// Passo o filtro de sobel no Ritmo Visual

	cvNamedWindow(vdo->getName());

	cvShowImage(vdo->getName(),visualRythim->data);

}
