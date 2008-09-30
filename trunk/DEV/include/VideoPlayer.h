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

	public:
		QMutex mutex;
		uchar* imageData;
		int imgWidth, imgHeight;

};
#endif
