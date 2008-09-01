#include "cv.h"
#include "highgui.h"
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

	double dy;
	int i = 0;
	double *array_dy;

	Log::writeLog("%s :: array[%x] size_i[%d]", __FUNCTION__, array, size_i);

	array_dy = (double*)malloc(sizeof(double)*size_i);

	// Calculo para a posicao 0
	array_dy[i] = ( 0 - array[i] ) / 2;
	Log::writeLog("%s :: array_dy[%d] = %ld", __FUNCTION__, i, array_dy[i]);

	for ( i = 1 ; i < size_i ; i++ )
	{
			array_dy[i] = (( array[i-1] - array[i+1] ) / 2);
			Log::writeLog("%s :: array_dy[%d] = %lf", __FUNCTION__, i, array_dy[i]);
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
	Fade *fade;
	Frame *frameFADE;
	VisualRythim *vrh;

	int len_i;
	int i;
	int j;

	// Array do VRH
	double *array_vrh;

	// Array da primeira derivada
	double *array_dy;

	double last_point = 0;
	int fade_start = 0;
	int fade_end = 0 ;
	int var = 0;
	double fade_max = 0;
	int fade_max_idx = 0;
	double aux = 0;

	Transition *transition;

	// Processo de detecção de transições do tipo FADE
	
	vrh = new VisualRythim();

	// 1- Pegamos o video e criamos o Ritmo Visual por Histograma
	array_vrh = vrh->createVRH(vdo);

	len_i = cvRound(vdo->getFramesTotal());

	// 2- Tiramos a derivada
	array_dy = fade->calcDerivative(array_vrh, len_i);

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

	// A partir dai é necessário fazer uma análize na função derivada.
	// O que iremos fazer é o seguinte:
	// Varreremos todo o array em buscas de variações muito que tenham uma duração relativamente grande
	// essa é a característica de um fade.
	// Se essa variação for toda positiva, então temos um fade-out, caso contrário, temos um fade-in.

	for ( i=0 ; i < len_i ; i++)
	{

		Log::writeLog("%s :: last_point [%lf] = array_dy[%d][%lf]", __FUNCTION__, last_point, i, array_dy[i]);

		if (last_point == array_dy[i])
		{

			if (fade_start == 1)
				fade_start = 0;

		}
		else
		{
			if (fade_start == 0)
			{
				Log::writeLog("%s :: fade_start in %d", __FUNCTION__, i);
				fade_start = i;
			}
			else if (array_dy[i] == 0.0 || i+1 >= len_i)
			{
				fade_end = i;

				var = fade_end - fade_start;

				Log::writeLog("%s :: %d - %d, total points : %d", __FUNCTION__, fade_start, fade_end, var);

				// Se for maior que um limiar
				// esse limiar foi definido realizando alguns experimentos onde apareciam
				// transicoes diferentes de fade.
				if (var> 5)
				{
					int type;
					char label[100];


					for (j=fade_start ; j<fade_end; j++)
					{
						if (fabs(array_dy[j]) > fade_max)
						{
							fade_max = array_dy[j];
							fade_max_idx = j;
						}

					}

					Log::writeLog("%s :: max value : idx : %d valor %lf", __FUNCTION__, fade_max_idx, fade_max);

					if (fade_max < 0)
					{
						type = TRANSITION_FADEIN;
						strcpy(label, "Fade In");
						Log::writeLog("%s :: fade in in : %d", __FUNCTION__, fade_max_idx);
					}
					else
					{
						type = TRANSITION_FADEOUT;
						strcpy(label, "Fade Out");
						Log::writeLog("%s :: fade out in : %d", __FUNCTION__, fade_max_idx);
					}

					// Cria o objeto da transição
					transition = new Transition(type, fade_max_idx, label);

					// Adiciona no container
					transitionList->push_back(*transition);

				}
				else
				{
					Log::writeLog("%s :: [%d] < [%d] - not a fade", __FUNCTION__, var, 5);
				}

				// Reseta controles
				fade_max = 0;
				fade_max_idx = 0;
				fade_start = fade_end = var = 0;
			}
		}

		// Ultimo ponto, igual a ponto corrente
		last_point = array_dy[i];
	}

	delete array_vrh;
	delete array_dy;
	delete vrh;
}
