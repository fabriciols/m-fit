#include "cv.h"
#include "highgui.h"

#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/Transition.h"
#include "../include/DetectTransitions.h"
#include "../include/Cut.h"
#include "../include/VisualRythim.h"

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
* 06/07/08 - Thiago Mizutani
* Ordenação das chamadas de função.
* 04/07/08 - Thiago Mizutani
* Criação.
************************************************************************/

Transition* DetectTransitions::detectTransitions(Video* vdo)
{ 
	VisualRythim *vr = 0;

	Frame *visualRythim = new Frame(vr->createVR(vdo));

	cvNamedWindow(vdo->getName());

	cvShowImage(vdo->getName(),visualRythim->data);
	
}
