/*

#include "../include/Video.h"
*/
#include "cv.h"
#include "highgui.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"

#include "../include/Video.h"
#include "../include/Transition.h"
#include "../include/DetectTransitions.h"

/************************************************************************
* Função que faz a detecção das transições.
*************************************************************************
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

/*
Transition* DetectTransitions::detectTransitions(Video* vdo)
{
	Transition* transitions[];

	// Detecção de cortes ou todas. 
	// Quando for deteccao de todos os tipos inicia-se pelo corte.
	//
	Cut* DTC;
//		Fade* DTF;
//		Dissolve* DTD;

	DTC->detectTransitions(type,vdo); // Cortes
//		DTF->detectTransitions(type,vdo); // Fades
//		DTD->detectTransitions(type,vdo); // Dissolve

	Cut* DTC;
	transitions = DTC->detectTransitions(type,vdo);

	return (transitions);
}
*/
