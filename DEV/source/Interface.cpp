#include "../ui_mfit.h" 
#include "../include/Interface.h"

#include "QWidget.h"
#include <QImage>
#include <QPainter>


mfit::mfit(QMainWindow *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
}

void mfit::on_okButton_clicked()
{
 	QImage image("C:\\TCC\\SRC\\DEV\\bin\\lena.jpg");

 	ui.videoLabel->setPixmap(QPixmap::fromImage(image));
}
