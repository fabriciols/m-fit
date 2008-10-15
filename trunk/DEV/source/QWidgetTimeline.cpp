#include "../include/QWidgetTimeline.h"
#include "cv.h"
#include "highgui.h"
#include "../include/Transition.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Time.h"
#include "../include/Log.h"
#include "../include/Video.h"
#include "../include/Project.h"
#include "../include/VideoPlayer.h"

extern Project* currentProject;
extern VideoPlayer* vdo_player;

QWidgetTimeline::QWidgetTimeline(QWidget *parent)
    : QWidget(parent)
{ }

void QWidgetTimeline::mousePressEvent(QMouseEvent *event)
{
	QPoint point(this->pos());
	QPoint pointEvent(event->pos());
	QRect  rect(this->geometry());

	if (currentProject->getVideo() == 0x0)
		return;

	Log::writeLog("%s :: widget x[%d] y[%d] press x[%d] y[%d]",
			__FUNCTION__,
			point.x(), point.y(),
			pointEvent.x(), pointEvent.y());

	if (pointEvent.x() >= 9 && pointEvent.x() <= rect.width() - 9)
	{
		Video *vdo;

		int posFrame;
		int x;

		// A widget tem 9 pixels antes de comecar realmente
		// a imagem da timeline
		x = pointEvent.x() - 9 ;

		vdo = currentProject->getVideo();

		//posFrame = cvRound(SIZE_SEC_FRAME*vdo->getFPS())*x / SIZE_FRAME_TIMELINE;
		posFrame = currentProject->TimelinePosToFrame(x);

		Log::writeLog("%s :: setting frame to pos [%d]", __FUNCTION__, posFrame);

		vdo->seekFrame(posFrame);

		if (!vdo_player->isRunning())
		{
			Frame *frame;
			frame = vdo->getCurrentFrame();
			vdo_player->updatePlayer(frame);

			delete frame;
		}
	}
	else
	{
		return;
	}

}
