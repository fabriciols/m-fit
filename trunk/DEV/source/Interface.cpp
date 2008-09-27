#include "../ui_mfit.h" 
#include "cv.h"
#include "highgui.h"


#include "QWidget.h"
#include <QImage>
#include <QPainter>
#include <QtGui>

#include "../include/Interface.h"

#include "../include/Transition.h"
#include "../include/Project.h"


extern Project *currentProject;

mfit::mfit(QMainWindow *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
}

void mfit::on_playButton_clicked()
{
	// Exemplo de como abrir uma imagem em um QLabel 
	// Frame *frame = new Frame("C:\\TCC\\SRC\\DEV\\bin\\lena.jpg");
 	//QImage *img;
	//img = frame->IplImageToQImage();
 	//ui.videoLabel->setPixmap(QPixmap::fromImage(*img));
	//

	Video *video;
	video = currentProject->getVideo();

	if (video == NULL)
		return;

	if (video->getCurrentPosition() < video->getFramesTotal())
		this->updateVideoPlayer(video->getNextFrame());

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

void mfit::on_actionLoadVideo_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this,
			"Open Video",
			".",
			"Suported Videos (*.avi *.wmv *.mpg)");

	if (!fileName.isEmpty())
	{
		currentProject->openVideo(fileName);
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

void mfit::changeWindowTitle(char *string)
{
	// Se tiver algum caracter na string
	if (string[0])
	{
		QString title(string);

		// Preenche o titulo
		this->setWindowTitle(string);
	}
	else
	{
		return;
	}
}

void mfit::insertVideoProperty(char *param_cy, char *value_cy)
{
	// Cria o item referente a Tree
	QTreeWidgetItem *item = new QTreeWidgetItem(this->ui.videoPropertiesTree);

	// Cria a lista de items
	QList<QTreeWidgetItem *> items;

	// Adiciona as colunas
	item->setText(0, param_cy);
	item->setText(1, value_cy);

	items.append(item);

	this->ui.transitionsTree->insertTopLevelItems(0, items);
}

void mfit::updateVideoPlayer(Frame* frame)
{
	QImage *image;
	image = frame->IplImageToQImage();

 	ui.videoLabel->setPixmap(QPixmap::fromImage(*image));
}
