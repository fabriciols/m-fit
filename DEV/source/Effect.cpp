#include "cv.h"
#include <QImage>
#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Effect.h"

long Effect::getFrameStart()
{
	return frameStart;
}

long Effect::getFrameEnd()
{
	return frameEnd;
}

int Effect::getID()
{
	return ID;
}

char *Effect::getName()
{
	return name_cy;
}

void Effect::setName(char *name)
{
	strcpy(this->name_cy, name);
}

void Effect::setID(int ID)
{
	this->ID = ID;
}

void Effect::setFrameStart(long pos)
{
	this->frameStart = pos;
}

void Effect::setFrameEnd(long pos)
{
	this->frameEnd = pos;
}
