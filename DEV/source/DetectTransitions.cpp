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
* Função que faz a detecção das transições.
*************************************************************************
* param (E): Video* vdo => video em qual será feita a detecção
* param (E): Transition* transitions => lista de transições
*************************************************************************
* return : Lista de transições.
*************************************************************************
* Histórico:
* 13/08/08 - Thiago Mizutani
* Implementação dos procedimentos de detecção das transições
* 04/07/08 - Thiago Mizutani
* Criação.
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
