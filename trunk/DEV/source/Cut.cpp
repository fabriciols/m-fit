#include "cv.h"
#include "highgui.h"

#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/Transition.h"
#include "../include/DetectTransitions.h"
#include "../include/Cut.h"
#include "../include/VisualRythim.h"

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
* 06/07/08 - Thiago Mizutani
* Ordena��o das chamadas de fun��o.
* 04/07/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

Transition* DetectTransitions::detectTransitions(Video* vdo)
{ 
	VisualRythim *vr = 0;

	Frame *visualRythim = new Frame(vr->createVR(vdo));

	cvNamedWindow(vdo->getName());

	cvShowImage(vdo->getName(),visualRythim->data);
	
}
