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
* 13/08/08 - Thiago Mizutani
* Chamadas das funções que completam o processo de detecção de cortes.
* 06/07/08 - Thiago Mizutani
* Ordenação das chamadas de função.
* 04/07/08 - Thiago Mizutani
* Criação.
************************************************************************/

Transition* Cut::detectTransitions(Video* vdo, int type)
{ 
   Frame* frameAux = 0;
	Filters* filters = 0;

	// Se a posicao do video não for a inicial, aponto para o primeiro frame.
	if (vdo->getCurrentPosition() != 0)
		vdo->seekFrame(0);

	VisualRythim *vr = 0;

	Frame *visualRythim = new Frame(vr->createVR(vdo));

	frameAux = filters->lowPass(visualRythim, 5);
	// Passo o filtro de sobel no Ritmo Visual

	cvNamedWindow(vdo->getName());

	cvShowImage(vdo->getName(),visualRythim->data);

}
