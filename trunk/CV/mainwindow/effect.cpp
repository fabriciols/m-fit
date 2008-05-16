#include "effect.h"

Effect::Effect(IplImage* img)
{
	this->img->cvImg = img;
	this->img->qtImg = NULL;
}

Effect::Effect(QImage* img)
{
	this->img->qtImg = img;
	this->img->cvImg = NULL;
}
