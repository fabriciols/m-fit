#include "../include/Time.h"

/************************************************************************
* Transforma de milesegundos para o formato HH:MM:SS.MMM
*************************************************************************
* param (E): unsigned long msec - Tempo em milisegundos
************************************************************************
* return: Nenhum.
************************************************************************
* Hist�rico
* 30/07/08 - Fabr�cio Lopes de Souza
* Cria��o.
************************************************************************/
void Time::setTime(unsigned long msec)
{
	this->msec = (int)( msec % 1000);
	this->sec  = (int)((msec /1000) % 60);
	this->min  = (int)((msec /60000) % 60);
	this->hour = (int)( msec /3600000);

	return;
}
