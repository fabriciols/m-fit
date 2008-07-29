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
* Fun��o que faz a detec��o das transi��es.
*************************************************************************
* param (E): int type => tipo de detec��o de transi��o a ser feita.
* 								 0 -> todas
* 								 1 -> s� cortes
* 								 2 -> s� fades
* 								 3 -> s� dissolve
*************************************************************************
* return : Lista de transi��es.
*************************************************************************
* Hist�rico:
* 04/07/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

/*
Transition* DetectTransitions::detectTransitions(Video* vdo)
{
	Transition* transitions[];

	// Detec��o de cortes ou todas. 
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
