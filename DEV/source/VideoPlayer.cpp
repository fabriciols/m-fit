#include "../include/VideoPlayer.h"

extern Project *currentProject;

void VideoPlayer::run()
{
	Video *vdo = currentProject->getVideo();
	Frame *frame;

	while (true)
	{

		if ((vdo->getCurrentPosition() >= vdo->getFramesTotal())
				|| vdo == 0x0 )
		{
			break;
		}

		frame = vdo->getNextFrame();

		if (frame == 0x0)
		{
			break;
		}

		emit renderedImage(frame);

		usleep(1000);
	}
}
