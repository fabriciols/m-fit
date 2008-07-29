#include "../include/Time.h"

void Time::setTime(unsigned long msec)
{
	this->msec = (int)( msec % 1000);
	this->sec  = (int)((msec /1000) % 60);
	this->min  = (int)((msec /60000) % 60);
	this->hour = (int)( msec /3600000);

	return;
}
