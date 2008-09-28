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

#include "../include/VideoPlayer.h"


extern Project *currentProject;
extern VideoPlayer *vdo_player;

mfit::mfit(QMainWindow *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	qRegisterMetaType<Frame>("Frame");
	connect(vdo_player, SIGNAL(renderedImage(Frame *)),
			this, SLOT(updateVideoPlayer(Frame *)));
}

void mfit::on_playButton_clicked()
{
	// Exemplo de como abrir uma imagem em um QLabel 
	// Frame *frame = new Frame("C:\\TCC\\SRC\\DEV\\bin\\lena.jpg");
	//QImage *img;
	//img = frame->IplImageToQImage();
	//ui.videoLabel->setPixmap(QPixmap::fromImage(*img));
	//
	vdo_player->start();

}

void mfit::on_pauseButton_clicked()
{
	if (vdo_player->isRunning())
		vdo_player->terminate();
}

void mfit::on_stopButton_clicked()
{
	Video *vdo;

	vdo = currentProject->getVideo();

	vdo->seekFrame(0);

	if (vdo_player->isRunning())
		vdo_player->terminate();
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

/*
	void mfit::updateVideoPlayer(Frame* frame)
	{
	QImage *image;
	image = frame->IplImageToQImage();

	QPixmap pix_image = QPixmap::fromImage(*image);

	ui.videoLabel->setScaledContents(true);
	ui.videoLabel->setPixmap(pix_image);

	}
	*/

/*
	void mfit::updateVideoPlayer()
	{

	Video *vdo = currentProject->getVideo();
	Frame *frame;

	if ((vdo->getCurrentPosition() >= vdo->getFramesTotal())
	|| vdo == 0x0 )
	{
	vdo_player->terminate();
	return;
	}

	frame = vdo->getNextFrame();

	if (frame == 0x0)
	{
	vdo_player->terminate();
	return;
	}

	updateVideoPlayer(frame);

	delete frame;
	}
	*/

void mfit::updateVideoPlayer(Frame *frame)
{
	QImage *image;

	image = frame->IplImageToQImage();

	QPixmap pix_image = QPixmap::fromImage(*image);

	ui.videoLabel->setScaledContents(true);
	ui.videoLabel->setPixmap(pix_image);

	delete frame;
	delete image;
}

/*
void mfit::updateVideoPlayer(QImage *image)
{
	QPixmap pix_image = QPixmap::fromImage(*image);

	ui.videoLabel->setScaledContents(true);
	ui.videoLabel->setPixmap(pix_image);
}
*/
