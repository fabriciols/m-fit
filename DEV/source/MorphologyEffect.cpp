#include "cv.h"
#include <QImage>

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Effect.h"
#include "../include/Morphology.h"
#include "../include/MorphologyEffect.h"

Erode::Erode()
{
	Erode(0, 0);
}

Dilate::Dilate()
{
	Dilate(0, 0);
}

Dilate::Dilate(long frameStart, long frameEnd)
{
	this->setName("Dilatacao");
	this->setID(EFFECT_DILATE);
	this->setFrameStart(frameStart);
	this->setFrameEnd(frameEnd);
}

Erode::Erode(long frameStart, long frameEnd)
{
	this->setName("Erosao");
	this->setID(EFFECT_ERODE);
	this->setFrameStart(frameStart);
	this->setFrameEnd(frameEnd);
}


Frame* Erode::applyEffect(Frame* frame)
{
	Frame *frameResult;
	Morphology *morph = new Morphology();

	frameResult = morph->erode(frame);

	delete morph;

	return frameResult;
}

Frame* Dilate::applyEffect(Frame* frame)
{
	Frame *frameResult;
	Morphology *morph = new Morphology();

	frameResult = morph->dilate(frame);

	delete morph;

	return frameResult;
}
