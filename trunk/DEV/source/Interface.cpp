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
#include "../include/Log.h"

#include "../include/VideoPlayer.h"


extern Project *currentProject;
extern VideoPlayer *vdo_player;

/************************************************************************
* Construtor da interface
*************************************************************************
* param (E): QMainWindow -> referencia da janela que criou o objeto
************************************************************************
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
mfit::mfit(QMainWindow *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	qRegisterMetaType<Frame>("Frame");
	connect(vdo_player, SIGNAL(renderedImage(Frame *)),
			this, SLOT(updateVideoPlayer(Frame *)));
}

/************************************************************************
* Tratar o evento do botão PLAY. Inicia a thread do video player
*************************************************************************
* param (E): Nenhum
************************************************************************
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
void mfit::on_playButton_clicked()
{
	vdo_player->start();
	return;
}

/************************************************************************
* Tratar o evento do botão PAUSE. Se a thread estiver rodando, mata ela
*************************************************************************
* param (E): Nenhum
************************************************************************
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
void mfit::on_pauseButton_clicked()
{
	if (vdo_player->isRunning())
		vdo_player->terminate();
}

/************************************************************************
* Tratar o evento do botão STOP. Se a thread estiver rodando, mata ela,
* e posiciona o ponteiro do video para seu inicio.
*************************************************************************
* param (E): Nenhum
************************************************************************
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
void mfit::on_stopButton_clicked()
{
	Video *vdo;

	vdo = currentProject->getVideo();

	vdo->seekFrame(0);

	if (vdo_player->isRunning())
		vdo_player->terminate();
}

/************************************************************************
* Tratar o evento do botão OpenProject.
* TODO: Ler o XML e subir os membros da estrutura Project.
*************************************************************************
* param (E): Nenhum
*************************************************************************
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
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
* Tratar o evento do botão LoadVideo.
* Abre o objeto Video, atualiza a videoProperties
*************************************************************************
* param (E): Nenhum
*************************************************************************
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
void mfit::on_actionLoadVideo_triggered()
{
	// Segundo  parametro - Mensagem que ira aparecer no topo da imagem
	// Terceiro parametro - Diretorio inicial
	// Quarto   parametro - Mensagem q ira aparecer la embaixo, e as extensões suportadas
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
* Tratar o evento do botão SaveAs.
* TODO: Nao faz porra nenhuma pq nao temos o ParserXML
*************************************************************************
* param (E): Nenhum
*************************************************************************
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
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
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
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
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
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
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
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

/************************************************************************
* Printa um Frame no videoLabel (Video Player)
*************************************************************************
* param (E): Nenhum
*************************************************************************
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
void mfit::updateVideoPlayer(Frame *frame)
{
	QImage *image;

	// Trava a thread do video_player
	vdo_player->mutex.lock();

	// Converte a imagem
	image = frame->IplImageToQImage();

	QPixmap pix_image = QPixmap::fromImage(*image);

	ui.videoLabel->setScaledContents(true);
	ui.videoLabel->setPixmap(pix_image);

	delete frame;
	delete image;

	// Destrava
	vdo_player->mutex.unlock();
}

/*
void mfit::updateVideoPlayer(QImage *image)
{
	QPixmap pix_image = QPixmap::fromImage(*image);

	ui.videoLabel->setScaledContents(true);
	ui.videoLabel->setPixmap(pix_image);
}
*/
