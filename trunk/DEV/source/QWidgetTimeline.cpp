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
		int n;
		int o;
		int pos;
		int p1;
		int p2;
		int x;

		x = pointEvent.x() - 9 ;

		n = rect.width() / 75;
		o = rect.width() % 75;

		vdo = currentProject->getVideo();

		p1 = rect.width() - o ;
		p2 = (o == 0 ? n : n-1) * cvRound(vdo->getFPS()) ;

		pos = ( x * p2 ) / p1;

		Log::writeLog("%s :: setting frame to pos [%d]", __FUNCTION__, pos);

		vdo->seekFrame(pos);


		if (!vdo_player->isRunning())
		{
			Frame *frame;
			frame = vdo->getCurrentFrame();
			vdo_player->updateVideo(frame);

			delete frame;
		}
	}
	else
	{
		return;
	}

}
