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

Transition* DetectTransitions::detectTransitions(int type, Video* vdo)
{
	Transition* transitions[];

	/**
	 * Detec��o de cortes ou todas. 
	 * Quando for deteccao de todos os tipos inicia-se pelo corte.
	 **/
	if (type == 0)
	{
		Cut* DTC;
//		Fade* DTF;
//		Dissolve* DTD;

		DTC->detectTransitions(type,vdo); // Cortes
//		DTF->detectTransitions(type,vdo); // Fades
//		DTD->detectTransitions(type,vdo); // Dissolve
	}
	else if(type == 1)
	{
		Cut* DTC;
		transitions = DTC->detectTransitions(type,vdo);
	}
	else if (type == 2)
	{
		// Fades
	}
	else if (type == 3)
	{
		// Dissolve
	}

	return (transitions);
}
