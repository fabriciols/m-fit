#include "cv.h"
#include "highgui.h"
#include <stdexcept>
#include <vector>
#include <QImage>

#include "../include/Time.h"
#include "../include/Log.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"

#include "../include/Video.h"
#include "../include/Transition.h"
#include "../include/DetectTransitions.h"
#include "../include/VisualRythim.h"

#include "../include/Dissolve.h"
#include "../include/Effect.h"
#include "../include/Color.h"

/************************************************************************
* Construtor que somente inicializa as variavies de controle com nulo 
*************************************************************************
* param (E): nenhum
*************************************************************************
* Hist�rico:
* 18/08/08 - Ivan Shiguenori Machida
* Cria��o.
************************************************************************/
Dissolve::Dissolve()
{
}

/************************************************************************
* Construtor que somente inicializa as variavies de controle com nulo 
*************************************************************************
* param (E): Video *vdo - guarda o video corrente para calculos de
* detec��o
*************************************************************************
* Hist�rico:
* 18/08/08 - Ivan Shiguenori Machida
* Cria��o.
************************************************************************/
Dissolve::Dissolve(Video *vdo)
{
	this->video = vdo;
}

/************************************************************************
* Fun��o que detecta as transi��es do tipo Dissolve
*************************************************************************
* param (E): Video* vdo -> video � detectar as transi��es
* param (S): Transition* transitions -> Posic�o corrente da lista de transi��es
*************************************************************************
* return : Nenhum
*************************************************************************
* Hist�rico:
* 16/10/08 - Mauricio Hirota
* Cria��o.
************************************************************************/
void Dissolve::detectTransitions(Video* vdo, std::vector<Transition>* transitionList)
{

	/**
	 * #####################################################################
	 * #####################################################################
	 *
	 * MAUR�CIO E IVAN. LEIAM ISSO ANTES DE FAZER QUALQUER COISA. 
	 *
	 * V�RIAS COISAS.
	 *
	 * 1 - Coloquem coment�rios no c�digo. vcs j� colocam uns nomes TOTALMENTE
	 * escrotos pras vari�veis do tipo: INT A, INT B. Nao custa nada dar uns
	 * nomes mais intuitivos e que fa�am mais sentido. Isso ajuda na manuteN
	 * ��o e pra mim e fabr�cio que n�o estamos fazendo o dissolve ou qqr outro
	 * fica muito mais claro o entendimento.
	 *
	 * Nada de var1, var2... escreve o nome daquilo que isso se refere porra
	 * VARIACE1, VARIANCE2. derivative em vez de der... hoje vcs lembram.. 
	 * amanha nem vao lembrar mais.
	 *
	 * 2 - REVEJAM TUDO aquilo que eu comentei. Tem monte de coisa que n�o
	 * faz muito sentido a�.. e se faz... bom... vcs N�O comentaram.
	 *
	 * 3 - Monte de coisa que vcs colocaram ficou estranho. Vcs t�o entrando
	 * em loops absurdamente grandes. N�o sei se � realmente necess�rio, mas
	 * se REALMENTE for... bom.. COMENTEM... mas comentem de forma DECENTE.
	 *
	 * 4 - N�o tenham a pregui�a de comentar... isso s� ajuda.
	 *
	 * 5 - N�o fiquem misturando conceitos. Se um for ou um processo serve
	 * pra fazer a fun��o X, ent�o que seja s� isso... se depois tiver que
	 * varrer td de novo... foda-se... varre... mas n�o misturem conceitos
	 * isso confunde.
	 *
	 * #####################################################################
	 * #####################################################################
	 **/

	Transition *transition;
	Frame* visual;
	double mean = 0;
  	double *variance;
	double *array_dvrh;
   double ratio;
	double var_2_der, med_1_der ;
	int len_i, i, j, k;

	Color *color = new Color();
	Frame* frameGray = 0;
	double *points;    //, *pontos_anterior, *valor_pontos,;  O que isso armazena?
	int pointsSize; 
   int *detect;
	//double *calculo_pontos=0;

	//char pri[4],seg[4],ter[4],qua[4];

	len_i = cvRound(vdo->getFramesTotal());

//	visual = vdo->getCurrentFrame(); Aqui vcs perdem o primeiro frame.

	pointsSize = (visual->getWidth()*visual->getHeight()); // Pra que serve isso? Vcs v�o varrer at� aonde com isso?

	for(k=0;k<len_i;k++)
	{
		visual = vdo->getNextFrame();
		frameGray = color->convert2Gray(visual);

		// Um conselho? N�o fa�am isso... cada parte do processo faz uma coisa..
		// n�o misturem essas coisas s� pra n�o ter que chamar depois... n�o � nem por processamento
		// mas isso atrapalha pra fazer manuten��o e entendimento das coisas. inclusive de vcs.
		// to falando do array_drvh.
		mean = array_dvrh[k] = frameGray->mediaBin(); 
		Log::writeLog("[%d] mean[%.lf]", k, mean);

		j=0; // vcs zeram o j toda hora pra que? Retirem isso....
		for(i=0;i<pointsSize;i++)
		{
			// Vcs s�o loco? points nem � um array. E se for vcs tem no��o de at� onde vcs t�o 
			// querendo varrer??? pointsSize pode chegar a mais de 100.000.... revisem isso... e por favor.
			// respondam a estas perguntas pra mim via e-mail. E oq vcs pretendem com esse c�culo?
			variance[k] += (points[i] - mean) * (points[i] - mean); 
		}

		variance[k] = variance[k] / (pointsSize-1);

		delete visual; // Isso evita o estouro de mem�ria.
		delete frameGray; // Sempre que vc for varrer o v�deo e ficar mudando o frame, deleta antes de pegar o pr�ximo;
		// A cada NextFrame que vc d� vc cria um objeto novo, o ponteiro aponta pra esse novo e n�o apaga o anterior.
		// deletem este coment�rio depois que lerem.
		
	}
	
	var_2_der = calcSecondDerivative(variance, len_i);
	med_1_der = calcSecondDerivative(array_dvrh, len_i);
	
	for(int w=0; w<len_i; w++)
	{
		ratio = calcRatioVarianceVRH(var_2_der, med_1_der);
		if (ratio<0)
			ratio = (-1)*ratio;
		// por enquanto usa 2
		// usa 2 oq!? Comentem o c�digo pelo amor d deus... n�o da pra entender nada do q vcs t�o fazendo assim...
		// desse jeito at� vcs acabam se perdendo depois e a gnt tbm n�o consegue ajudar em nada.
		if (ratio < 2)
			detect[w] = 1;
		else
			detect[w] = 0;
	}
	// varre os pontos de deteccao e pega o primeiro entre 5 ou mais
	for(i=0; i<len_i; i++)
	{
		if(detect[i] == 1)
		{
			for(j=i; j<i+5; j++)
			{
				if (detect[j] == 0)
					detect[i]=0;
			}
			transition = new Transition(TRANSITION_DISSOLVE, i, "Dissolve");
			transitionList->push_back(*transition);

		}
		// se for ponto de deteccao, zera os proximos 5 pontos e avnca o contador para o sexto elemento
		if(detect[i] == 1)
		{
			for (k=i; k<i+5; k++)
			{
				detect[k] = 0;
			}
			i = i+5;
		}
	}
     
}

/************************************************************************
* Fun��o que calcula a variancia atrav�s do histograma do ritmo visual
*************************************************************************
* param (E): vetor de m�dia
*************************************************************************
* return : float -> valor da vari�ncia do histograma do ritmo visual
*************************************************************************
* Hist�rico:
* 19/08/08 - Ivan Shiguenori Machida
* Efetuando os calculos para variancia e desvio padrao.
* 17/08/08 - Ivan Shiguenori Machida
* Cria��o.
************************************************************************/
double Dissolve::calcVariance(double *arrayM, int size_i )
{
	int i;
	double mean = 0;
	double variance=0;
   double soma=0;
	// faz a m�dia
	for (i=1; i<=size_i; i++)
	{
		soma=soma + arrayM[i];
	}
	mean = soma / size_i;
	
	// faz variancia
	for (i=1; i<=size_i; i++)
	{
		variance = variance + ((arrayM[i] - mean ) * (arrayM[i] - mean));
	}
	variance = variance / (size_i-1);
	return(variance);
}

/************************************************************************
* Fun��o que calcula a segunda derivada atrav�s do histograma do ritmo 
* visual
*************************************************************************
* param (E): vetor de entrada
*************************************************************************
* return : float -> valor da segunda derivada do histograma do ritmo 
* visual
*************************************************************************
* Hist�rico:
* 18/08/08 - Ivan Shiguenori machida
* Cria��o.
************************************************************************/
double Dissolve::calcSecondDerivative(double *array, int size)
{
	double *derivative2 = (double *)malloc(sizeof(double)*size);
	double *derivative1 = (double *)malloc(sizeof(double)*size);
	int i;
	// Calcula segunda derivada, se for primeiro ou ultimo frame, derivada  = 0
	for (i=1; i<=size; i++)
	{
		if (i == 1 || i == size)
			derivative1[i] = 0;
		else
			derivative1[i] = (array[i+1] + array[i-1])/2;
	}
	for (i=1; i<=size; i++)
	{
		if (i == 1 || i == size)
			derivative2[i] = 0;
		else
	
		derivative2[i] = (derivative1[i+1] + derivative1[i-1])/2;
	}
	
	return(*derivative2);	
}

/************************************************************************
* Fun��o que calcula a raz�o entre variancia e a segunda derivada gerada 
* atrav�s do histograma do ritmo visual
*************************************************************************
* param (E): variancia e segunda derivada
*************************************************************************
* return : float -> valor raz�o entre variancia e a segunda derivada 
* gerada atrav�s do histograma do ritmo visual
*************************************************************************
* Hist�rico:
* 18/08/08 - Ivan Shiguenori machida
* Cria��o.
************************************************************************/
double Dissolve::calcRatioVarianceVRH(double variance, double secondDerivative)
{
	if (secondDerivative!=0)
		return(variance / secondDerivative);
	else
		return 0;
}
