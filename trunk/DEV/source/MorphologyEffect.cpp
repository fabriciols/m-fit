#include "cv.h"
#include <QImage>

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Effect.h"
#include "../include/Morphology.h"
#include "../include/MorphologyEffect.h"

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
