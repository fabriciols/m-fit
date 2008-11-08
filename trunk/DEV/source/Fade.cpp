#include "cv.h"
#include "highgui.h"
#include <QImage>
#include <vector>

#include "../include/Time.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"

#include "../include/Video.h"
#include "../include/Transition.h"
#include "../include/VisualRythim.h"
#include "../include/DetectTransitions.h"

#include "../include/Fade.h"

#include "../include/Log.h"

/************************************************************************
 * Função que calcula a derivada no exito y de um dado array
 * retorna um array com esta derivada.
 *************************************************************************
 * param (E): double array -> array a ser derivado no eixo y
 *************************************************************************
 * return : double* -> array com seus valores derivados
 *************************************************************************
 * Histórico:
 * 14/08/08 - Fabrício Lopes de Souza
 * Criação.
 ************************************************************************/
double* Fade::calcDerivative(double *array, int size_i)
{
	// Para calcular a derivada no eixo y , calculamos para cada ponto fy:
	// dy = ((f(y+1) - f(y-1)) / 2)

	//double dy;
	int i = 0;
	double *array_dy;

	Log::writeLog("%s :: array[%x] size_i[%d]", __FUNCTION__, array, size_i);

	array_dy = (double*)malloc(sizeof(double)*size_i);

	// Calculo para a posicao 0
	array_dy[i] = ( 0 - array[i] ) / 2;
	Log::writeLog("%s :: array_y[%d] = %lf array_dy[%d] = %lf", __FUNCTION__, i, array[i], i, array_dy[i]);

	for ( i = 1 ; i < size_i ; i++ )
	{
		array_dy[i] = (( array[i+1] - array[i-1] ) / 2);
		Log::writeLog("%s :: array_y[%d] = %lf array_dy[%d] = %lf", __FUNCTION__, i, array[i], i, array_dy[i]);
	}

	return array_dy;
}

/************************************************************************
 * Função que detecta as transições do tipo FADE (i.e. : fade-in e fade-out)
 *************************************************************************
 * param (E): Video* vdo -> video à detectar as transições
 * param (S): Transition* transitions -> Posicão corrente da lista de transições
 *************************************************************************
 * return : Nenhum
 *************************************************************************
 * Histórico:
 * 14/08/08 - Fabrício Lopes de Souza
 * Criação.
 ************************************************************************/
void Fade::detectTransitions(Video* vdo, std::vector<Transition>* transitionList)
{
	// Objeto FADE
	Fade *fade;

	// Objeto para o RVH
	VisualRythim *vrh;

	// Tamanho do video em frames
	int len_i;

	// Contadores
	int i;
	int j;

	// Array do VRH
	double *array_vrh;

	// Array da primeira derivada
	double *array_dy;

	// Ultimo ponto analizado
	double last_point = 0;

	// Ultimo ponto que teve variacao 0
	int last_zero = -99;

	// Poicao em que comecamos a analizar um fade
	int fade_start = 0;

	// Posicao aonde encerramos a analise de um fade
	int fade_end = 0 ;

	// Quantidade de frames presentes na transição
	int var = 0;

	// Variacao do ponto atual com o ponto anterior
	double var_d = 0.0;

	double fade_max = 0;
	int fade_max_idx = 0;

	// Quantidade de pontos que nao tiveram variacao
	int no_var = 0;

	// Posicao do fade
	int fade_pos = 0;

	// Objeto Transicao
	Transition *transition = 0x0;

	// Processo de detecção de transições do tipo FADE
	vrh = new VisualRythim();

	// 1- Pegamos o video e criamos o Ritmo Visual por Histograma
	array_vrh = vrh->createVRH(vdo);

	// Removemos partes indesejaveis
	vdo->removeWide();
	vdo->removeBorder();

	len_i = cvRound(vdo->getFramesTotal());

	// 2- Tiramos a derivada
	array_dy = fade->calcDerivative(array_vrh, len_i);

	// Descomentar para obter uma imagem do RVH derivado
	/*
	{
		int i;
		double *array_dy_aux = 0;

		array_dy_aux = (double*)malloc(sizeof(double)*len_i);


		// DUMP - cria a imagem do rvh, somente para analizarmos
		for (i = 0 ; i < len_i ; i ++)
		{
			array_dy_aux[i] = array_dy[i] + 50;
		}

		Frame *frameVRH = new Frame(array_dy_aux, len_i, 256);

		frameVRH->write("vrh_dump.jpg");

		delete frameVRH;
		delete array_dy_aux;
	}
	*/

	// A partir dai é necessário fazer uma análize na função derivada.
	// O que iremos fazer é o seguinte:
	// Varreremos todo o array em buscas de variações muito que tenham uma duração relativamente grande
	// essa é a característica de um fade.
	// Se essa variação for toda positiva, então temos um fade-out, caso contrário, temos um fade-in.

	// Desconsideramos o primeiro ponto da derivada
	// pois ele sempre será '0'
	last_point = array_dy[1];

	for ( i=2 ; i < len_i ; i++)
	{

		// Reseta controles
		fade_max = 0;
		fade_max_idx = 0;
		fade_end = var = 0;

		var_d = fabs(fabs(array_dy[i]) - fabs(last_point));

		if (fade_start > 0)
		{
			if (var_d <= 0.0000009)
			{
				no_var++;
			}
		}
		else
		{
			no_var = 0;
		}

		if (fabs(array_dy[i]) <= 0.009)
			last_zero = i;

		// Condições para se iniciar a verificação de um FADE
		if (
				fade_start == 0 &&
				var_d > 0.0 &&
				(i - last_zero <= 4 || i <= 4 ) &&
				array_dy[i] != 0.0
			)
		{
			Log::writeLog("%s :: fade_start in %d", __FUNCTION__, i);

			fade_start = i;

			fade_end = var = 0;
		} // Condições para se terminar um fade
		else if ( fade_start > 0 &&
				( array_dy[i] == 0.0 || ( i+1 >= len_i && var_d < 2.0 )))
		{
			fade_end = i - 1;

			var = fade_end - fade_start;

			Log::writeLog("%s :: %d - %d, total points : %d", __FUNCTION__, fade_start, fade_end, var);
			Log::writeLog("%s :: no_var [%d] var/2 [%d]", __FUNCTION__, no_var, var/2);

			// Se um 1/3 dos pontos forem 0
			// evidencia-se que não é fade
			if (no_var >= ( var / 3 ) )
			{
				Log::writeLog("%s :: to many 0 var, not a fade ( no_var [%d] var/3 [%d]", __FUNCTION__, no_var, var/3);
				no_var = 0;
				fade_start = 0;
				continue;
			}

			// Se for maior que um limiar
			// esse limiar foi definido realizando alguns experimentos onde apareciam
			// transicoes diferentes de fade.
			if (var > 0)
			{
				int type;
				char label[100];

				for (j = fade_start ; j <= fade_end; j++)
				{

					if (fabs(array_dy[j]) > fabs(fade_max))
					{
						fade_max = array_dy[j];
						fade_max_idx = j;
					}

				}

				Log::writeLog("%s :: max value : idx : %d valor %lf", __FUNCTION__, fade_max_idx, fade_max);

				// Cria o objeto da transição
				// Se o ponto maximo for negativo: temos um fade-out
				// caso contrario, temos um fade-in
				if (fade_max < 0)
				{
					type = TRANSITION_FADEOUT;
					strcpy(label, "Fade-Out");
					Log::writeLog("%s :: fade out: %d", __FUNCTION__, fade_pos);
					fade_pos = fade_end;
				}
				else
				{
					type = TRANSITION_FADEIN;
					strcpy(label, "Fade-In");
					Log::writeLog("%s :: fade in: %d", __FUNCTION__, fade_pos);
					fade_pos = fade_start;
				}

				transition = new Transition(type, fade_pos, label);

				/**
				 * Verifico se na posição em que eu detectei um corte já não foi considerada
				 * outro tipo de transição. Isso evita que o sistema diga que em uma mesma posição
				 * existam 2 transições diferentes.
				**/
				if( this->validateTransition((long)fade_pos, transitionList) )
					transitionList->push_back(*transition);

				fade_start = 0;

				no_var = 0;

			}
			else
			{
				fade_start = 0;
				no_var = 0;
				Log::writeLog("%s :: [%d] < [%d] - not a fade", __FUNCTION__, var, 8);
			}

		}

		// Ultimo ponto, igual a ponto corrente
		last_point = array_dy[i];
	}

	// Reseta o ROI craido pelo removeWide e removeBorder
	vdo->resetROI();

	delete array_vrh;
	delete array_dy;
	delete vrh;
}
