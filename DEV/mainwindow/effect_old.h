#include "cv.h"
#include <QImage>

class Image
{
	public:
		Image(char filename_cy[30]);
		Image(IplImage *img);
		Image(Image* img);

		Image* toGray(void);

		QImage* getQTimage();
		IplImage* getCVimage();

		char name[30];

	protected:
		IplImage* cvImage;
		QImage* qtImage;
};


class Effect
{
	public:
		Effect(Image* img);
		Effect(char* filename_cy);

		Image* img;
		Image* dilate();
};
