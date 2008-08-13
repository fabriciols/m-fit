/*

#include "../include/Video.h"
*/
#include "cv.h"
#include "highgui.h"

#include "../include/Histogram.h"
#include "../include/Time.h"
#include "../include/Frame.h"

#include "../include/Video.h"
#include "../include/Transition.h"
#include "../include/DetectTransitions.h"
#include "../include/Cut.h"

/************************************************************************
* Função que faz a detecção das transições.
*************************************************************************
* param (E): Video* vdo => video em qual será feita a detecção
* param (E): int type => tipo de detecção de transição a ser feita.
* 								 0 -> todas
* 								 1 -> só cortes
* 								 2 -> só fades
* 								 3 -> só dissolve
*************************************************************************
* return : Lista de transições.
*************************************************************************
* Histórico:
* 04/07/08 - Thiago Mizutani
* Criação.
************************************************************************/

Transition* DetectTransitions::detectTransitions(Video* vdo, int type)
{
	Transition* transitions;

	// Detecção de cortes ou todas. 
	// Quando for deteccao de todos os tipos inicia-se pelo corte.
	//
	Cut* DTC;
//		Fade* DTF;
//		Dissolve* DTD;
	
	if (type == 0 || type == 1)
		transitions = DTC->detectTransitions(vdo, type); // Cortes
//	else if (type == 2)
//		DTF->detectTransitions(type,vdo); // Fades
//	else if (type == 3)
//		DTD->detectTransitions(type,vdo); // Dissolve


	return (transitions);
}
