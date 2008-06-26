#include "cv.h"
#include "highgui.h"

#include <stdexcept>

#include "../include/Histogram.h"
#include "../include/Frame.h"

Frame::Frame(char *filename_cy)
{
	IplImage* img;
	img = cvLoadImage(filename_cy, 1);

	if (img == NULL)
	{
		throw "Cant open file";
	}

	// Guardamos a instância do IplImage na estrutura
	// do frame.
	this->data = img;

	// Aponta para os valores do IplImage
	this->width =  &this->data->width;
	this->height = &this->data->height;

}

Frame::Frame(IplImage *img_src)
{
	// Guardamos a instância do IplImage na estrutura
	// do frame.
	this->data = img_src;

	// Aponta para os valores do IplImage
	this->width =  &this->data->width;
	this->height = &this->data->height;
}

Frame::Frame(Frame *frame)
{
	// Faz a copia do objeto
	IplImage *img = new IplImage();

	memcpy(img, frame->data, sizeof(IplImage));

	this->data = img;

	this->width =  &this->data->width;
	this->height = &this->data->height;

}

// Retorna um FRAME em escala de cinza
// referente ao frame corrente
Frame* Frame::convert2Gray()
{
	IplImage *imgGray;

	// Cria a img com as mesmas dimensões da instância atual
	// porém com 8 bits e 1 canal apenas
	imgGray = cvCreateImage(cvGetSize(this->data),8,1);

	// Ordena os bytes da imagem, senão ela ficará de ponta cabeça
	imgGray->origin = this->data->origin;

	// Converte pra cinza;
	cvCvtColor(this->data, imgGray, CV_RGB2GRAY);

	return (new Frame(imgGray));
}

int Frame::getWidth()
{
	return(*this->width);
}

int Frame::getHeight()
{
	return(*this->height);
}
