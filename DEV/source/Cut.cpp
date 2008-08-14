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
* Fun��o que faz a detec��o das transi��es do tipo corte.
*************************************************************************
* param (E): int type => tipo de detec��o de transi��o a ser feita.
* 								 0 -> todas
* 								 1 -> s� cortes
*				 Video* vdo => v�deo onde ser� feita a detec��o
*************************************************************************
* return : Lista de transi��es.
*************************************************************************
* Hist�rico:
* 13/08/08 - Thiago Mizutani
* Chamadas das fun��es que completam o processo de detec��o de cortes.
* 06/07/08 - Thiago Mizutani
* Ordena��o das chamadas de fun��o.
* 04/07/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

Transition* Cut::detectTransitions(Video* vdo, int type)
{ 
   Frame* vrLow= 0;
	Frame* borderMap = 0;
	Filters* filters = 0;
	int treshould = 0;

	// Se a posicao do video n�o for a inicial, aponto para o primeiro frame.
	if (vdo->getCurrentPosition() != 0)
		vdo->seekFrame(0);

	VisualRythim *vr = 0;

	Frame *visualRythim = new Frame(vr->createVR(vdo));

	// Fa�o a suaviza��o do RV para retirada de ru�dos.
	vrLow = filters->lowPass(visualRythim, 5);

	// Passo o filtro de sobel no RV suavizado para destacar as bordas
	borderMap = this->createBorderMap(vrLow);

	// Pergunto ao usuario se deseja alterar a limiar para detec��o.
	treshould = this->defineTreshould(visualRythim->getHeight());
	
	//countPoints(borderMap, treshould);
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

Frame* Cut::createBorderMap(Frame* visualRythim)
{
	Filters* sobel = 0;
	Frame* borderMap = 0;

	Log::writeLog("%s :: visualRythim[%x]", __FUNCTION__, visualRythim);

	borderMap = sobel->Sobel(visualRythim,5);

	return (borderMap);
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

int Cut::defineTreshould(int height)
{
	int treshould = 0;
	Cut* cut = 0;

	// Would you like to set a new treshould? precisaremos do QT pra fazer isso
	// mas podemos fazer o msm esquema q a gnt faz com coletaED. s� preciso
	// ver como faz q eu nao sei. waitKey?
	
	setTreshould(treshould > 0 ? treshould : height/2);
	
	return (getTreshould());

}

/************************************************************************
* Fun��o que faz contagem dos pontos de cada coluna que possuem 
*************************************************************************
* param (E): Frame* borderMap => mapa de bordas do qual ser� feita a 
* 											contagem dos pontos
* param (E): int treshould => Valor do limiar para considera��es de onde
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

int Cut::getTreshould(void)
{

	return (this->treshould);

}

/************************************************************************
* Fun��o que seta o valor do limiar (private treshould) da classe CUT
*************************************************************************
* param (E): int treshould => Valor do limiar
*************************************************************************
* return :  nenhum.
*************************************************************************
* Hist�rico:
* 13/08/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

int Cut::setTreshould(int treshould)
{
	this->treshould = treshould;

	return 0;
}
