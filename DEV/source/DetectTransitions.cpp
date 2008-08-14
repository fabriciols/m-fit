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
#include "../include/Fade.h"
#include "../include/Fadein.h"
#include "../include/Fadeout.h"
#include "../include/Dissolve.h"

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

void DetectTransitions::detectTransitions(Video* vdo, Transition *transitions)
{
	// Detec��o todas as transi��es
	Cut* DTC;
	Fade* DTF;
	Dissolve* DTD;
	
	DTC->detectTransitions(vdo, transitions); // Cortes
	//DTF->detectTransitions(vdo, transitions); // Fade
	//DTD->detectTransitions(vdo, transitions); // Dissolve
}
