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
* Fun��o que faz a detec��o das transi��es.
*************************************************************************
* param (E): Video* vdo => video em qual ser� feita a detec��o
* param (E): int type => tipo de detec��o de transi��o a ser feita.
* 								 0 -> todas
* 								 1 -> s� cortes
* 								 2 -> s� fades
* 								 3 -> s� dissolve
*************************************************************************
* return : Lista de transi��es.
*************************************************************************
* Hist�rico:
* 13/08/08 - Thiago Mizutani
* Implementa��o dos procedimentos de detec��o das transi��es
* 04/07/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

Transition* DetectTransitions::detectTransitions(Video* vdo, int type)
{
	Transition* transitions;

	// Detec��o de cortes ou todas. 
	Cut* DTC;
//		Fade* DTF;
//		Dissolve* DTD;
	
	// Quando for deteccao de todos os tipos (0) inicia-se pelo corte.
	if (type == 0 || type == 1)
		transitions = DTC->detectTransitions(vdo, type); // Cortes
//	else if (type == 2)
//		DTF->detectTransitions(type,vdo); // Fades
//	else if (type == 3)
//		DTD->detectTransitions(type,vdo); // Dissolve


	return (transitions);
}
