#include "cv.h"
#include "highgui.h"
#include <vector>

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
* param (E): Transition* transitions => lista de transi��es
*************************************************************************
* return : Lista de transi��es.
*************************************************************************
* Hist�rico:
* 13/08/08 - Thiago Mizutani
* Implementa��o dos procedimentos de detec��o das transi��es
* 04/07/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

void DetectTransitions::detectTransitions(Video* vdo, std::vector<Transition>* transitionList)

{
	Cut* DTC = new Cut();
	Fade* DTF = new Fade();
//	Dissolve* DTD;
	
	DTC->detectTransitions(vdo, transitionList); // Cortes
	DTF->detectTransitions(vdo, transitionList); // Fade
	//DTD->detectTransitions(vdo, transitions); // Dissolve
}
