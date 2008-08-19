#include <stdio.h>
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
* Histórico:
* 15/08/08 - Thiago Mizutani
* Criação.
************************************************************************/

Cut::Cut()
{
	this->threshold = 0;	
}

/************************************************************************
* Função que faz a detecção das transições do tipo corte.
*************************************************************************
* param (E): Video* vdo => vídeo onde será feita a detecção
* param (E): Transition *transitions => lista de transições
*************************************************************************
* return : Lista de transições.
*************************************************************************
* Histórico:
* 16/08/08 - Thiago Mizutani
* Inclusão das chamadas das funções para processamento da imagem do RV.
* Criação do RV, suavização, mapa de bordas, binarização e contagem dos
* pontos.
* 13/08/08 - Thiago Mizutani
* Chamadas das funções que completam o processo de detecção de cortes.
* 06/07/08 - Thiago Mizutani
* Ordenação das chamadas de função.
* 04/07/08 - Thiago Mizutani
* Criação.
************************************************************************/

void Cut::detectTransitions(Video* vdo, Transition *transitions)
{ 
   Frame* vrLow = new Frame(); // Ritmo Visual suavizado
	Frame* borderMap = new Frame(); // Mapa de bordas
	Frame* binFrame = new Frame(); // Imagem binarizada
	Frame* frameAux = new Frame();
	Frame* visual= new Frame();

	Filters* filters = new Filters();

	VisualRythim *vr = new VisualRythim();

	Histogram* hist = new Histogram();

	Time* time = new Time();

	/**
	 *  Este objeto não poderá ser deletado no final da função, senão
	 *  irei deletar a última e a penúltima posição da lista.
	**/
	Transition* oldTransition = new Transition(); 
	
	char* label; 
	int threshold = 0;
	int thresholdBin = 0;
	int *trans = 0;
	double fps = vdo->getFPS();
	double totalFrames = vdo->getFramesTotal();

	label = (char*)malloc(sizeof(char)*19);

	// Se a posicao do video não for a inicial, aponto para o primeiro frame.
	if (vdo->getCurrentPosition() != 0)
		vdo->seekFrame(0);

	// Crio o Ritmo VIsual do vídeo
	visual = vr->createVR(vdo);

	Frame *visualRythim = new Frame(visual);

	/**
	 * Gero o histograma do RV para saber qual a maior luminancia presente 
	 * no RV para cálculo do limiar da contagem de pontos.
	**/
	hist = visualRythim->createHistogram();

	// Faço a suavização do RV para retirada de ruídos.
	vrLow = filters->lowPass(visualRythim, 5);

	// Passo o filtro de sobel no RV suavizado para destacar as bordas
	borderMap = this->createBorderMap(vrLow);

	// Pergunto ao usuario se deseja alterar a limiar para detecção.
	threshold = this->defineThreshold(visualRythim->getHeight());

	// Defino o limiar para binarização da imagem.
	thresholdBin = (hist->getMaxLuminance())/4;

	// Binarizo a imagem (transformo tudo em preto e branco)
	binFrame = frameAux->binarizeImage(borderMap,thresholdBin);

	// Realizo a contagem dos pontos das bordas que foram encontradas
	trans = countPoints(binFrame, threshold);
	
	for( int i=0; i<(int)totalFrames; i++ )
	{
		if(trans[i])
		{
			// É o primeiro
			if (!(transitions->previous))
			{
				transitions->previous = 0; // Se for o primeiro, não tem previous
				transitions->next = 0; // A nova transição da lista nunca tem next
				transitions->setPosTransition(i); // Informo a posição física (frame) em que ocorre a transição
				
				time->pos2time(i,fps); // Converto de posição física para tempo
				
				sprintf(label,"Cut in: %d:%d:%d:%d",time->getHour(),time->getMin(),time->getSec(),time->getMsec());
				
				transitions->setLabel(label); //Salvo a label de exibição da transição
				
				oldTransition = transitions; // Salvo a transição anterior
			}
			else // A lista já foi iniciada
			{
				Transition* newTransition = new Transition(); //Crio um novo objeto para inserir na lista

				// Este novo item é o próximo do item anterior
				oldTransition->next = newTransition; 

				newTransition->next = 0; // O novo item não tem próximo
				newTransition->previous = oldTransition; // O anterior do novo é o item anterior (comentário inútil...)
				newTransition->setPosTransition(i); // Guardo a posição (física) da ocorrência da transição

				// Monto a label para exibir na timeline
				sprintf(label,"Cut in: %d:%d:%d:%d",time->getHour(),time->getMin(),time->getSec(),time->getMsec());
				
				newTransition->setLabel(label); //Salvo a label de exibição da transição
				
				/**
				 *  Esta transição será a transição antiga da próxima transição (se houver). Por isso
				 *  não posso dar um delete na memória do oldTransition, senão limpo a área de memória que este
				 *  ponteiro está apontando, fazendo com que eu perca a última transição.
				**/
				oldTransition = newTransition; 
			}
		}
	}

	delete vrLow;
	delete borderMap;
	delete binFrame;
	delete frameAux;
	delete visualRythim;
	delete filters;
	delete vr;
	delete hist;
	delete time;

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
	Filters* sobel = new Filters();
	Frame* borderMap = new Frame();

	Log::writeLog("%s :: visualRythim[%x]", __FUNCTION__, visualRythim);

	// Crio o mapa de bordas do RV com o operador Sobel.
	borderMap = sobel->Sobel(visualRythim,2);

	delete sobel;
	delete borderMap;

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

int Cut::defineThreshold(int height)
{
	int threshold = 0;
	Cut* cut = 0;

	// Would you like to set a new threshold? precisaremos do QT pra fazer isso
	// mas podemos fazer o msm esquema q a gnt faz com coletaED. só preciso
	// ver como faz q eu nao sei. waitKey?
	
	setThreshold(threshold > 0 ? threshold : height/2);
	
	Log::writeLog("%s :: threshold(%d) ", __FUNCTION__, threshold);
	
	return (getThreshold());

}

/************************************************************************
* Função que faz contagem dos pontos de cada coluna que possuem 
*************************************************************************
* param (E): Frame* borderMap => mapa de bordas do qual será feita a 
* 											contagem dos pontos
* param (E): int threshold => Valor do limiar para considerações de onde
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

	/**
	 *	Varro toda a imagem coluna por coluna, pixel a pixel, verificando se
	 *	o pixel é branco. Se for branco, significa que faz parte da borda.
	 *	Ao término da contagem de pontos de uma coluna, verifico se o numero
	 *	de pontos da borda é maior ou igual ao valor do limiar. Se for marco 
	 *	no vetor transitions que aquela coluna representa um corte.	
	 * **/
	for (column = 0; column < width; column++)	
	{
		for (int y=0; y < height; y++)
		{
			luminance = borderMap->getPixel(column,y);
			if(luminance == 255);
			{
				Log::writeLog("%s :: luminance[%d] ", __FUNCTION__, luminance);
				points++;	
			}
		}
		Log::writeLog("%s :: tamanho da reta[%d] ", __FUNCTION__, points);
		// Se o nro de pontos da reta for > que o limiar, então é corte.
		transitions[column] = points >= threshold ? 1 : 0;
		points = 0;
	}
	
	return (transitions);	
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

int Cut::getThreshold(void)
{

	return (this->threshold);

}

/************************************************************************
* Função que seta o valor do limiar (private threshold) da classe CUT
*************************************************************************
* param (E): int threshold => Valor do limiar
*************************************************************************
* return :  nenhum.
*************************************************************************
* Histórico:
* 13/08/08 - Thiago Mizutani
* Criação.
************************************************************************/

int Cut::setThreshold(int threshold)
{
	this->threshold = threshold;

	return 0;
}
