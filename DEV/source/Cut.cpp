#include "cv.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Transition.h"
#include "../include/DetectTransitions.h"
#include "../include/Cut.h"

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
* 04/07/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

Transition* DetectTransitions::detectTransitions(int type, Video* vdo)
{ 
	// Upcast!!!
	VisualRythim* vr = 0;
	IplImage* visualRythim = 0;
	
	visualRythim = vr->createVR(vdo);

	
}
