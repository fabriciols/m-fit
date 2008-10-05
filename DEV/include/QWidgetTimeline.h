#ifndef QWIDGETIMELINE_INCLUDE
#define QWIDGETIMELINE_INCLUDE
#include <QtGui>

class QWidgetTimeline : public QWidget
{
	Q_OBJECT

	public:
		QWidgetTimeline(QWidget *parent = 0);

	protected:
		void mousePressEvent(QMouseEvent *event);
};
#endif
