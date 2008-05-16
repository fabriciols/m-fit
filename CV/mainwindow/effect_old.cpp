#include "effect.h"
#include "cv.h"

Image::IplImage* getCVimage()
{
	if (this.cvImage == null)
	{
		printf(":(");
	}

	return this.cvImage;
}


Image::Image* toGray(void)
{
	Image* img;

	IplImage *cvimg = this.getCVimage();

	cvCvtColor(getCVimage(), cvimg, CV_RGB2GRAY);

	img = new Image(cvimg);

	return img;
}

Image :: Image(char filename_cy[30])
{
	this.cvImage = cvLoadImage(filename_cy,1);
}

Image::Image(IplImage *img)
{
	this.cvImage = img;
}

Image::Image(Image* img)
{
	this.cvImage = get;
	this.qtImage = img.qtImage;
}

/* EFFECT */

Effect::Image* dilate()
{
	Image* imgEffect = new Image();

	imgEffect->cvImage = cvCreateImage(cvGetSize(this.img->cvImage),8,1);

	cvDilate(this.img->getCVimage(), imgEffect->cvImage, 0, 1);

	return imgEffect;
}

Effect::Effect(char *filename_cy)
{
	this.img = new Image(filename_cy);
}

Effect::Effect(Image* img)
{
	this.img = img;
}
