#include "cv.h"
#include "image.h"

class Effect
{
	public:
		Image* img;

		Effect(IplImage *img);
		Effect(QImage* img);
};
