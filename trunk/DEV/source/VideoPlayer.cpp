#include "../include/VideoPlayer.h"

extern Project *currentProject;

/************************************************************************
* Metodo que e executado quando a thread do VideoPlayer for iniciada.
* Pega frame a frame e emite via signal para a interface desenhar.
*************************************************************************
* param (E): Nenhum
*************************************************************************
* Hist�rico
* 29/09/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
void VideoPlayer::run()
{
	Video *vdo = currentProject->getVideo();
	Frame *frame = 0x0;

	if (vdo == 0x0)
		return;

	while (true)
	{

		// Condi��es de parada
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

		// Manda o sinal para desenhar o frame no Video Player
		emit renderedImage(frame);

		usleep(1000);

	}
}
