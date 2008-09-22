#include "../ui_mfit.h" 
#include "cv.h"

#include "../include/Interface.h"

#include "QWidget.h"
#include <QImage>
#include <QPainter>

#include "../include/Histogram.h"
#include "../include/Frame.h"


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
}
