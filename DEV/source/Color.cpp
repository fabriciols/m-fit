#include "cv.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Effect.h"
#include "../include/Color.h"

/************************************************************************
* Função que converte o frame para escala de cinza.
*************************************************************************
* param (E): Nenhum.
*************************************************************************
* return : Frame* - Retorna um ponteiro para um novo frame.
*************************************************************************
* Histórico:
* 27/06/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
Frame* Color::convert2Gray(Frame* frame)
{
	IplImage *imgGray;

	// Cria a img com as mesmas dimensões da instância atual
	// porém com 8 bits e 1 canal apenas
	imgGray = cvCreateImage(cvGetSize(frame->data),8,1);

	// Ordena os bytes da imagem, senão ela ficará de ponta cabeça
	imgGray->origin = frame->data->origin;

	// Converte pra cinza;
	cvCvtColor(frame->data, imgGray, CV_RGB2GRAY);

	return (new Frame(imgGray));
}
