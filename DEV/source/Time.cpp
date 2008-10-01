#include "../include/time.h"
#include "cv.h"

/************************************************************************
* Construtor da classe: inicializa as variaveis da classe com 0.
*************************************************************************
* param (E): Nenhum.
************************************************************************
* return: Nenhum.
************************************************************************
* Hist�rico
* 16/08/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

Time::Time()
{
	this->hour = 0;
	this->min = 0;
	this->sec = 0;
	this->msec = 0;
}

/************************************************************************
* Construtor da classe: inicializa as variaveis.
*************************************************************************
* param (E): int hora, int min, int sec, int msec
************************************************************************
* return: Nenhum.
************************************************************************
* Hist�rico
* 16/08/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Time::Time(int hour, int min, int sec, int msec = 0)
{
	this->hour = hour;
	this->min = min;
	this->sec = sec;
	this->msec = msec;
}

/************************************************************************
* Construtor da classe: converte de posicao de frame para time
*************************************************************************
* param (E): Nenhum.
************************************************************************
* return: Nenhum.
************************************************************************
* Hist�rico
* 16/08/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Time::Time(double framePos, double fps)
{
	// Converte pra segundos, depois pra mili-segundos
	this->setTime(cvRound((framePos / fps)*1000));
}

/***********************************************************************
* transforma de posi��o f�sica para o formato hh:mm:ss.mmm
************************************************************************
* param (e) : int position => posi��o do frame no v�deo
* param (e) : double fps => frames por segundo do v�deo
************************************************************************
* return: nenhum.
************************************************************************
* hist�rico
* 15/08/08 - thiago mizutani
* cria��o.
************************************************************************/

void Time::pos2time(int position, double fps)
{
	long milisec = 0;

	milisec = position/(int)fps;

	this->setTime(milisec);
}

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
/***********************************************************************
* Retorna o campo hora do tempo da transi��o.
************************************************************************
* param (e) : nenhum
************************************************************************
* return: campo hora
************************************************************************
* hist�rico
* 15/08/08 - thiago mizutani
* cria��o.
************************************************************************/

int Time::getHour()
{
	return (this->hour);
}

/***********************************************************************
* Retorna o campo minutos do tempo da transi��o.
************************************************************************
* param (e) : nenhum
************************************************************************
* return: campo minutos
************************************************************************
* hist�rico
* 15/08/08 - thiago mizutani
* cria��o.
************************************************************************/

int Time::getMin()
{
	return (this->min);
}

/***********************************************************************
* Retorna o campo segundos do tempo da transi��o.
************************************************************************
* param (e) : nenhum
************************************************************************
* return: campo segundos 
************************************************************************
* hist�rico
* 15/08/08 - thiago mizutani
* cria��o.
************************************************************************/

int Time::getSec()
{
	return (this->sec);
}

/***********************************************************************
* Retorna o campo milisegundos do tempo da transi��o.
************************************************************************
* param (e) : nenhum
************************************************************************
* return: campo milisegundos
************************************************************************
* hist�rico
* 15/08/08 - thiago mizutani
* cria��o.
************************************************************************/

int Time::getMsec()
{
	return (this->msec);
}

/***********************************************************************
* Retorna o numero do frame referente ao tempo.
************************************************************************
* param (e) : double fps -> Fps referente ao tempo
************************************************************************
* return: Posicao em frame de video
************************************************************************
* hist�rico
* 15/08/08 - Fabricio Lopes de Souza
* cria��o.
************************************************************************/
unsigned long Time::getFramePos(double fps)
{
	unsigned long total;
	unsigned long pos;

	total = 0;

	total =  this->sec;
	total += this->min  * 60;
	total += this->hour * 60 * 60;

	pos = cvRound(total * fps);

	return pos;

}
