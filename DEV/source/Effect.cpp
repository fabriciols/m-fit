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
