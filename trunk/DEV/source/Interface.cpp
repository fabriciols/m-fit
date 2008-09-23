#include "../ui_mfit.h" 
#include "cv.h"
#include "highgui.h"

#include "../include/Interface.h"

#include "QWidget.h"
#include <QImage>
#include <QPainter>
#include <QtGui>

#include "../include/Time.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/Project.h"

extern Project *currentProject;


mfit::mfit(QMainWindow *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
}

void mfit::on_okButton_clicked()
{
	Frame *frame = new Frame("C:\\TCC\\SRC\\DEV\\bin\\lena.jpg");

 	QImage *img;

	img = frame->IplImageToQImage();

 	ui.videoLabel->setPixmap(QPixmap::fromImage(*img));

	QString fileName = QFileDialog::getOpenFileName(this);
}

void mfit::on_actionOpenProject_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this);

	if (!fileName.isEmpty())
	{
		currentProject = new Project((char*)&fileName);
	}
}
