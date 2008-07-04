#include "cv.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Transition.h"
#include "../include/DetectTransitions.h"
#include "../include/Cut.h"

/************************************************************************
* Função que faz a detecção das transições do tipo corte.
*************************************************************************
* param (E): int type => tipo de detecção de transição a ser feita.
* 								 0 -> todas
* 								 1 -> só cortes
*				 Video* vdo => vídeo onde será feita a detecção
*************************************************************************
* return : Lista de transições.
*************************************************************************
* Histórico:
* 04/07/08 - Thiago Mizutani
* Criação.
************************************************************************/

Transition* DetectTransitions::detectTransitions(int type, Video* vdo)
{ 
	// Upcast!!!
	VisualRythim* vr = 0;
	IplImage* visualRythim = 0;
	
	visualRythim = vr->createVR(vdo);

	
}
