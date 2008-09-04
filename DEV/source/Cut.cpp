#include <stdio.h>
#include <vector>
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
#include "../include/Log.h"

/************************************************************************
* Construtor
*************************************************************************
* param (E): nenhum
*************************************************************************
* return : nenhum
*************************************************************************
* Hist�rico:
* 15/08/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

Cut::Cut()
{
	this->threshold = 0;	
}

/************************************************************************
* Fun��o que faz a detec��o das transi��es do tipo corte.
*************************************************************************
* param (E): Video* vdo => v�deo onde ser� feita a detec��o
* param (E): Transition *transitions => lista de transi��es
*************************************************************************
* return : Lista de transi��es.
*************************************************************************
* Hist�rico:
* 16/08/08 - Thiago Mizutani
* Inclus�o das chamadas das fun��es para processamento da imagem do RV.
* Cria��o do RV, suaviza��o, mapa de bordas, binariza��o e contagem dos
* pontos.
* 13/08/08 - Thiago Mizutani
* Chamadas das fun��es que completam o processo de detec��o de cortes.
* 06/07/08 - Thiago Mizutani
* Ordena��o das chamadas de fun��o.
* 04/07/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

void Cut::detectTransitions(Video* vdo, std::vector<Transition>* transitionList)
{ 
	Frame* visualRythim= new Frame();

	Filters* filters = new Filters();

	VisualRythim *vr = new VisualRythim();

	Time* time = new Time();

	/**
	 *  Este objeto n�o poder� ser deletado no final da fun��o, sen�o
	 *  irei deletar a �ltima e a pen�ltima posi��o da lista.
	**/
	Transition* oldTransition = new Transition(); 
	
	char* label; 
	int threshold = 0;
	int thresholdBin = 0;
	int *trans = 0;
	double fps = vdo->getFPS();
	double totalFrames = vdo->getFramesTotal();

	label = (char*)malloc(sizeof(char)*19);

	// Se a posicao do video n�o for a inicial, aponto para o primeiro frame.
	if (vdo->getCurrentPosition() != 0)
		vdo->seekFrame(0);

	// Crio o Ritmo VIsual do v�deo
	visualRythim = vr->createVR(vdo);

	visualRythim->removeWide();

	Log::writeLog("%s :: new height = %d", __FUNCTION__, visualRythim->getHeight());

	// Passo o filtro de sobel no RV suavizado para destacar as bordas
	this->createBorderMap(visualRythim);

	// Pergunto ao usuario se deseja alterar a limiar para detec��o.
	threshold = this->defineThreshold(visualRythim->getHeight());

	Log::writeLog("%s :: threshold[%d]", __FUNCTION__, threshold);	

	// Defino o limiar para binariza��o da imagem.
	thresholdBin = (visualRythim->getMaxLum())/4;
	
	Log::writeLog("%s :: maxluminance[%d]", __FUNCTION__, visualRythim->getMaxLum());	
	Log::writeLog("%s :: thresholdbin[%d]", __FUNCTION__, thresholdBin);	

	// Binarizo a imagem (transformo tudo em preto e branco)
	visualRythim->binarizeImage(thresholdBin);

	// Realizo a contagem dos pontos das bordas que foram encontradas
	trans = countPoints(visualRythim, threshold);
	
	for( int i=0; i<(int)totalFrames; i++ )
	{
		if(trans[i]) 
		{
			
			sprintf(label, "Cut in: %d:%d:%d:%d",time->getHour(),time->getMin(),time->getSec(),time->getMsec());

			Log::writeLog("%s :: Cut in: %d", __FUNCTION__, i);

			Transition* newTransition = new Transition(TRANSITION_CUT,i,label);

			// Adiciona no container
			transitionList->push_back(*newTransition);

			/*  MODO ANTIGO (LISTA LIGADA!!!)
	
			// � o primeiro
			if (!(transitions->previous))
			{
				transitions->previous = 0; // Se for o primeiro, n�o tem previous
				transitions->next = 0; // A nova transi��o da lista nunca tem next
				transitions->setPosTransition(i); // Informo a posi��o f�sica (frame) em que ocorre a transi��o
				
				time->pos2time(i,fps); // Converto de posi��o f�sica para tempo
				
				sprintf(label,"Cut in: %d:%d:%d:%d",time->getHour(),time->getMin(),time->getSec(),time->getMsec());
				
				Log::writeLog("%s :: %s", __FUNCTION__, label);		

				transitions->setLabel(label); //Salvo a label de exibi��o da transi��o
				
				oldTransition = transitions; // Salvo a transi��o anterior
			}
			else // A lista j� foi iniciada
			{
				Transition* newTransition = new Transition(); //Crio um novo objeto para inserir na lista

				// Este novo item � o pr�ximo do item anterior
				oldTransition->next = newTransition; 

				newTransition->next = 0; // O novo item n�o tem pr�ximo
				newTransition->previous = oldTransition; // O anterior do novo � o item anterior (coment�rio in�til...)
				newTransition->setPosTransition(i); // Guardo a posi��o (f�sica) da ocorr�ncia da transi��o

				// Monto a label para exibir na timeline
				sprintf(label,"Cut in: %d:%d:%d:%d",time->getHour(),time->getMin(),time->getSec(),time->getMsec());
				
				Log::writeLog("%s :: %s", __FUNCTION__, label);		

				newTransition->setLabel(label); //Salvo a label de exibi��o da transi��o
				
				/ *  Esta transi��o ser� a transi��o antiga da pr�xima transi��o (se houver). Por isso
				 *  n�o posso dar um delete na mem�ria do oldTransition, sen�o limpo a �rea de mem�ria que este
				 *  ponteiro est� apontando, fazendo com que eu perca a �ltima transi��o.
				 */
		/*
			oldTransition = newTransition; 
			}
		*/
		}
	}

	delete visualRythim;
	delete filters;
	delete vr;
	delete time;

}

/************************************************************************
* Fun��o que retorna o mapa de bordas de um Ritmo Visual
*************************************************************************
* param (E): Frame* visualRythim => Ritmo Visual do qual ser� gerado o
*  											mapa de bordas
*************************************************************************
* return : mapa de bordas.
*************************************************************************
* Hist�rico:
* 13/08/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

void Cut::createBorderMap(Frame* visualRythim)
{
	Filters* canny = new Filters();

	Log::writeLog("%s :: visualRythim[%x]", __FUNCTION__, visualRythim);

	// Crio o mapa de bordas do RV com o operador Sobel.
//   sobel->Sobel(visualRythim,0);

	canny->Canny(visualRythim, 20, 200, 3);

	delete canny;

}

/************************************************************************
* Fun��o que retorna um novo limiar para detec��o de cortes. Caso o 
* usu�rio tenha setado um novo valor retorna este valor, sen�o retorna 0
* e o sistema ir� considerar 
*************************************************************************
* param (E): Frame* visualRythim => Ritmo Visual do qual ser� gerado o
*  											mapa de bordas
*************************************************************************
* return : mapa de bordas.
*************************************************************************
* Hist�rico:
* 13/08/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

int Cut::defineThreshold(int height)
{
	int threshold = 0;
	Cut* cut = 0;

	// Would you like to set a new threshold? precisaremos do QT pra fazer isso
	// mas podemos fazer o msm esquema q a gnt faz com coletaED. s� preciso
	// ver como faz q eu nao sei. waitKey?
	
	setThreshold(threshold > 0 ? threshold : height/2);
	
	Log::writeLog("%s :: threshold(%d) ", __FUNCTION__, this->threshold);
	
	return (getThreshold());

}

/************************************************************************
* Fun��o que faz contagem dos pontos de cada coluna que possuem 
*************************************************************************
* param (E): Frame* borderMap => mapa de bordas do qual ser� feita a 
* 											contagem dos pontos
* param (E): int threshold => Valor do limiar para considera��es de onde
* 										existem cortes
*************************************************************************
* return : array preenchido com 1 ou 0, onde houver corte ser� preenchido
* com 1 e onde n�o houver com 0. Cada coluna do mapa de bordas representa
* 1 frame do v�deo. Com isso ser� poss�vel saber onde est�o os cortes.
*************************************************************************
* Hist�rico:
* 13/08/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

int* Cut::countPoints(Frame* borderMap, int threshold)
{
	// Coluna do mapa de bordas que estou analisando.
	int column = 0;
	int points = 0;
	int width = borderMap->getWidth();
	int height = borderMap->getHeight();

	int* transitions;
	int luminance = 0;	

	transitions = (int*)malloc(sizeof(int)*width);
	memset(transitions,'\0',width);
	
	Log::writeLog("%s :: threshold[%d] ", __FUNCTION__, threshold);
	Log::writeLog("%s :: width[%d] ", __FUNCTION__, width);
	Log::writeLog("%s :: height[%d] ", __FUNCTION__, height);

	/**
	 *	Varro toda a imagem coluna por coluna, pixel a pixel, verificando se
	 *	o pixel � branco. Se for branco, significa que faz parte da borda.
	 *	Ao t�rmino da contagem de pontos de uma coluna, verifico se o numero
	 *	de pontos da borda � maior ou igual ao valor do limiar. Se for marco 
	 *	no vetor transitions que aquela coluna representa um corte.	
	 * **/
	for (column = 0; column < width; column++)	
	{
		for (int y=0; y < height; y++)
		{
			luminance = borderMap->getPixel(column,y);
			if(luminance == 255)
			{
				points++;	
			}
		}
		Log::writeLog("%s :: tamanho da reta_%d[%d] ", __FUNCTION__, column, points);
		// Se o nro de pontos da reta for > que o limiar, ent�o � corte.
		transitions[column] = points >= threshold ? 1 : 0;
		points = 0;
	}
	
	return (transitions);	
}

/*************************************************************************
* Fun��o que retorna o valor do limiar escolhido para a contagem de pontos
**************************************************************************
* param (E): nenhum
**************************************************************************
* return : Treshould
**************************************************************************
* Hist�rico:
* 13/08/08 - Thiago Mizutani
* Cria��o.
*************************************************************************/

int Cut::getThreshold(void)
{

	return (this->threshold);

}

/************************************************************************
* Fun��o que seta o valor do limiar (private threshold) da classe CUT
*************************************************************************
* param (E): int threshold => Valor do limiar
*************************************************************************
* return :  nenhum.
*************************************************************************
* Hist�rico:
* 13/08/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

int Cut::setThreshold(int threshold)
{
	this->threshold = threshold;

	return 0;
}
