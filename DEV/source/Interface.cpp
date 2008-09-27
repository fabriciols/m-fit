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
#include "../include/Transition.h"
#include "../include/Project.h"

extern Project *currentProject;

mfit::mfit(QMainWindow *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
}

void mfit::on_okButton_clicked()
{
	/* Exemplo de como abrir uma imagem em um QLabel 
	Frame *frame = new Frame("C:\\TCC\\SRC\\DEV\\bin\\lena.jpg");
 	QImage *img;
	img = frame->IplImageToQImage();
 	ui.videoLabel->setPixmap(QPixmap::fromImage(*img));
	*/

	return;

}

void mfit::on_actionOpenProject_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this);

	if (!fileName.isEmpty())
	{
		currentProject->openProject((char*)&fileName);
	}
	else
	{
		return;
	}
}

void mfit::on_actionSaveAs_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(this);

	if (!fileName.isEmpty())
	{
		return;
	}
}

void mfit::on_openVideoButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this);

	if (!fileName.isEmpty())
	{
		if (currentProject == NULL)
			currentProject->openProject((char*)&fileName);
	}
	else
	{
		return;
	}
}

void mfit::changeWindowTitle(char *string)
{
	// Se tiver algum caracter na string
	if (string[0])
	{
		QString title(string);

		// Preenche o titulo
		this->setWindowTitle(title);
	}
	else
	{
		return;
	}
}
