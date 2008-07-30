#include "../include/Time.h"

/************************************************************************
* Transforma de milesegundos para o formato HH:MM:SS.MMM
*************************************************************************
* param (E): unsigned long msec - Tempo em milisegundos
************************************************************************
* return: Nenhum.
************************************************************************
* Histórico
* 30/07/08 - Fabrício Lopes de Souza
* Criação.
************************************************************************/
void Time::setTime(unsigned long msec)
{
	this->msec = (int)( msec % 1000);
	this->sec  = (int)((msec /1000) % 60);
	this->min  = (int)((msec /60000) % 60);
	this->hour = (int)( msec /3600000);

	return;
}
