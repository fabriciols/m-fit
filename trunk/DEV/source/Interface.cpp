#include "../ui_mfit.h" 
#include "cv.h"
#include "highgui.h"


#include "QWidget.h"
#include <QImage>
#include <QPainter>
#include <QtGui>

#include "../include/Interface.h"
#include "../include/Time.h"

#include "../include/Transition.h"
#include "../include/Project.h"
#include "../include/Log.h"

#include "../include/VideoPlayer.h"


extern Project *currentProject;
extern VideoPlayer *vdo_player;

/************************************************************************
* Construtor da interface
*************************************************************************
* param (E): QMainWindow -> referencia da janela que criou o objeto
************************************************************************
* Hist�rico
* 29/09/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
mfit::mfit(QMainWindow *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	qRegisterMetaType<Frame>("Frame");
	connect(vdo_player, SIGNAL(renderedImage(QImage *)),
			this, SLOT(updateVideoPlayer(QImage *)));
}

/************************************************************************
* Tratar o evento do bot�o PLAY. Inicia a thread do video player
*************************************************************************
* param (E): Nenhum
************************************************************************
* Hist�rico
* 29/09/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
void mfit::on_playButton_clicked()
{
	if (!vdo_player->isRunning())
		vdo_player->start();

	return;
}

/************************************************************************
* Tratar o evento do bot�o PAUSE. Se a thread estiver rodando, mata ela
*************************************************************************
* param (E): Nenhum
************************************************************************
* Hist�rico
* 29/09/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
void mfit::on_pauseButton_clicked()
{
	if (vdo_player->isRunning())
		vdo_player->terminate();
}

/************************************************************************
* Tratar o evento do bot�o STOP. Se a thread estiver rodando, mata ela,
* e posiciona o ponteiro do video para seu inicio.
*************************************************************************
* param (E): Nenhum
************************************************************************
* Hist�rico
* 29/09/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
void mfit::on_stopButton_clicked()
{
	Video *vdo;

	if (vdo_player->isRunning())
	{
		vdo_player->terminate();

		vdo = currentProject->getVideo();

		vdo->seekFrame(0);
	}

}

/************************************************************************
 * Tratar o evento do bot�o OpenProject.
 * TODO: Ler o XML e subir os membros da estrutura Project.
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Hist�rico
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/
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

/************************************************************************
 * Tratar o evento do bot�o LoadVideo.
 * Abre o objeto Video, atualiza a videoProperties
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Hist�rico
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/
void mfit::on_actionLoadVideo_triggered()
{
	// Segundo  parametro - Mensagem que ira aparecer no topo da imagem
	// Terceiro parametro - Diretorio inicial
	// Quarto   parametro - Mensagem q ira aparecer la embaixo, e as extens�es suportadas
	QString fileName = QFileDialog::getOpenFileName(this,
			"Open Video",
			".",
			"Supported Videos (*.avi)");

	if (!fileName.isEmpty())
	{
		currentProject->openVideo(fileName);
	}
	else
	{
		return;
	}
}

/************************************************************************
 * Tratar o evento do bot�o SaveAs.
 * TODO: Nao faz porra nenhuma pq nao temos o ParserXML
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Hist�rico
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/
void mfit::on_actionSaveAs_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(this);

	if (!fileName.isEmpty())
	{
		return;
	}
}

/************************************************************************
 * Altera o Title da janela principal.
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Hist�rico
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/
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

/************************************************************************
 * Limpa as entradas da videoPropertiesTree
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Hist�rico
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/
void mfit::clearVideoProperty()
{
	Log::writeLog("%s :: clear videoPropertiesTree", __FUNCTION__); 
	this->ui.videoPropertiesTree->clear();
}

/************************************************************************
 * Adiciona uma entrada na videoPropertiesTree
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Hist�rico
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/
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

/************************************************************************
 * Printa um Frame no videoLabel (Video Player)
 *************************************************************************
 * param (E): Frame *frame -> Frame a ser impresso no video player
 *************************************************************************
 * Hist�rico
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/
void mfit::updateVideoPlayer(Frame *frame)
{
	QImage *image;

	// Converte a imagem
	image = frame->IplImageToQImage();

	updateVideoPlayer(image);

	delete frame;
	delete image;

}

/************************************************************************
 * Printa um QImage no videoLabel (Video Player)
 *************************************************************************
 * param (E): QImage *image -> Imagem a ser impressa no video player
 *************************************************************************
 * Hist�rico
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/
void mfit::updateVideoPlayer(QImage *image)
{
	// Trava a thread do video_player
	Video *vdo = 0;
	vdo = currentProject->getVideo();

	setVideoTime(vdo->getCurrentPosition(), vdo->getFPS());

	QPixmap pix_image = QPixmap::fromImage(*image);

	ui.videoLabel->setScaledContents(true);
	ui.videoLabel->setPixmap(pix_image);

	// Destrava
	//	vdo_player->mutex.unlock();
}

void mfit::setVideoTime(double framePos, double fps)
{
	Time *cvTime = new Time(framePos, fps);

	const QTime *time = new QTime(
			cvTime->getHour(),
			cvTime->getMin(),
			cvTime->getSec(),
			cvTime->getMsec());

	ui.videoTime->setTime(*time) ;

	delete cvTime;
	delete time;

}


void mfit::on_videoTime_timeChanged(const QTime & time)
{
	return;

	/*
	Video *vdo;
	Time *cvTime;

	if (vdo_player->isRunning())
		return;

	vdo = currentProject->getVideo();

	cvTime = new Time(time.hour(), time.minute(), time.second(), time.msec());

	vdo->seekFrame(cvTime->getFramePos(vdo->getFPS()));

	updateVideoPlayer(vdo->getCurrentFrame());
	*/

}
