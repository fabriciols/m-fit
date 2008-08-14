#include "cv.h"
#include "highgui.h"

#include "../include/Time.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"

#include "../include/Video.h"
#include "../include/Transition.h"
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
	// dy = ( f(y+1) - f(y-1) ) / 2

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
			Log::writeLog("%s :: array_dy[%d] = %ld", i, array_dy[i]);
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
void detectTransitions(Video* vdo, Transition *transitions)
{
}

