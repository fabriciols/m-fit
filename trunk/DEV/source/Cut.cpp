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
   Frame* vrLow= 0;
	Frame* borderMap = 0;
	Filters* filters = 0;
	int treshould = 0;

	// Se a posicao do video não for a inicial, aponto para o primeiro frame.
	if (vdo->getCurrentPosition() != 0)
		vdo->seekFrame(0);

	VisualRythim *vr = 0;

	Frame *visualRythim = new Frame(vr->createVR(vdo));

	// Faço a suavização do RV para retirada de ruídos.
	vrLow = filters->lowPass(visualRythim, 5);

	// Passo o filtro de sobel no RV suavizado para destacar as bordas
	borderMap = this->createBorderMap(vrLow);

	// Pergunto ao usuario se deseja alterar a limiar para detecção.
	treshould = this->defineTreshould(visualRythim->getHeight());
	
	//countPoints(borderMap, treshould);
}

/************************************************************************
* Função que retorna o mapa de bordas de um Ritmo Visual
*************************************************************************
* param (E): Frame* visualRythim => Ritmo Visual do qual será gerado o
*  											mapa de bordas
*************************************************************************
* return : mapa de bordas.
*************************************************************************
* Histórico:
* 13/08/08 - Thiago Mizutani
* Criação.
************************************************************************/

Frame* Cut::createBorderMap(Frame* visualRythim)
{
	Filters* sobel = 0;
	Frame* borderMap = 0;

	Log::writeLog("%s :: visualRythim[%x]", __FUNCTION__, visualRythim);

	borderMap = sobel->Sobel(visualRythim,5);

	return (borderMap);
}

/************************************************************************
* Função que retorna um novo limiar para detecção de cortes. Caso o 
* usuário tenha setado um novo valor retorna este valor, senão retorna 0
* e o sistema irá considerar 
*************************************************************************
* param (E): Frame* visualRythim => Ritmo Visual do qual será gerado o
*  											mapa de bordas
*************************************************************************
* return : mapa de bordas.
*************************************************************************
* Histórico:
* 13/08/08 - Thiago Mizutani
* Criação.
************************************************************************/

int Cut::defineTreshould(int height)
{
	int treshould = 0;
	Cut* cut = 0;

	// Would you like to set a new treshould? precisaremos do QT pra fazer isso
	// mas podemos fazer o msm esquema q a gnt faz com coletaED. só preciso
	// ver como faz q eu nao sei. waitKey?
	
	setTreshould(treshould > 0 ? treshould : height/2);
	
	return (getTreshould());

}

/************************************************************************
* Função que faz contagem dos pontos de cada coluna que possuem 
*************************************************************************
* param (E): Frame* borderMap => mapa de bordas do qual será feita a 
* 											contagem dos pontos
* param (E): int treshould => Valor do limiar para considerações de onde
* 										existem cortes
*************************************************************************
* return : array preenchido com 1 ou 0, onde houver corte será preenchido
* com 1 e onde não houver com 0. Cada coluna do mapa de bordas representa
* 1 frame do vídeo. Com isso será possível saber onde estão os cortes.
*************************************************************************
* Histórico:
* 13/08/08 - Thiago Mizutani
* Criação.
************************************************************************/

int Cut::countPoints(Frame* borderMap, int treshould)
{
	// Coluna do mapa de bordas que estou analisando.
/*	int column = 0;

	for (column = 0; column<borderMap->getWidth(); column++)	
	{
		for (int y=0; y<borderMap->getHeight(); y++)
		{
			if(borderMap->getPixel(column,y)	);
		}
	}
*/
	return 0;	
}

/*************************************************************************
* Função que retorna o valor do limiar escolhido para a contagem de pontos
**************************************************************************
* param (E): nenhum
**************************************************************************
* return : Treshould
**************************************************************************
* Histórico:
* 13/08/08 - Thiago Mizutani
* Criação.
*************************************************************************/

int Cut::getTreshould(void)
{

	return (this->treshould);

}

/************************************************************************
* Função que seta o valor do limiar (private treshould) da classe CUT
*************************************************************************
* param (E): int treshould => Valor do limiar
*************************************************************************
* return :  nenhum.
*************************************************************************
* Histórico:
* 13/08/08 - Thiago Mizutani
* Criação.
************************************************************************/

int Cut::setTreshould(int treshould)
{
	this->treshould = treshould;

	return 0;
}
