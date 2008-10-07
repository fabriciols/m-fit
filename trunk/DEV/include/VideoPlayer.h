#include <QThread>
#include <QMutex>
#include "QMainWindow"

#include "../include/Interface.h"
#include "../include/Transition.h"
#include "../include/Project.h"

#ifndef VIDEOPLAYER_INCLUDE
#define VIDEOPLAYER_INCLUDE

class VideoPlayer: public QThread
{
	Q_OBJECT 

	protected:
		void run();

	signals:
		void renderedImage(QImage *image);
		void renderedImage(QImage *image, int type);
		void renderedImage(QImage *image, QImage *imageHist);

	public:
		QMutex mutex;
		uchar* imageData;
		uchar* histData;
		uchar* timelineData;
		VideoPlayer();

		int imgWidth, imgHeight;
		int histWidth, histHeight;
		int timelineWidth, timelineHeight;
		void updateHist(Frame *frame);
		void updateVideo(Frame *frame);

		Frame* frameTimeline;
		Frame* frameTimelineEdited;


};
#endif