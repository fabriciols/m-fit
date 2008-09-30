#include "../include/VideoPlayer.h"

extern Project *currentProject;

/************************************************************************
* Metodo que e executado quando a thread do VideoPlayer for iniciada.
* Pega frame a frame e emite via signal para a interface desenhar.
*************************************************************************
* param (E): Nenhum
*************************************************************************
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
void VideoPlayer::run()
{
	Video *vdo = currentProject->getVideo();
	Frame *frame = 0x0;
	QImage *image = 0x0;

	// Variaveis internas
	this->imageData = 0x0;
	this->imgWidth  = 0x0;
	this->imgHeight = 0x0;

	if (vdo == 0x0)
		return;

	while (true)
	{

		// Condições de parada
		if ((vdo->getCurrentPosition() >= vdo->getFramesTotal()))
		{
			break;
		}

		// Pega o proximo frame.
		frame = vdo->getNextFrame();

		// Se nao vir nada, aborta
		if (frame == 0x0)
		{
			break;
		}

		image = frame->IplImageToQImage(&imageData, &imgWidth, &imgHeight);

		delete frame;

		// Manda o sinal para desenhar o frame no Video Player
		emit renderedImage(image);

		usleep(1000);

	}
}
