#include "../ui_mfit.h" 
#include "cv.h"
#include "highgui.h"


#include "QWidget.h"
#include <QImage>
#include <QPainter>
#include <QtGui>

#include "../include/Effect.h"
#include "../include/Interface.h"
#include "../include/Time.h"

#include "../include/Transition.h"
#include "../include/Project.h"
#include "../include/Log.h"
#include "../include/Color.h"
#include "../include/Histogram.h"

#include "../include/VideoPlayer.h"

#include "../include/DetectTransitions.h"
#include "../include/Cut.h"
#include "../include/Fade.h"
#include "../include/Dissolve.h"

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

	connect(vdo_player, SIGNAL(renderedImage(QImage *, int)),
			this, SLOT(updateVideoPlayer(QImage *, int)));

	connect(vdo_player, SIGNAL(renderedImage(QImage *, QImage *)),
			this, SLOT(updateVideoPlayer(QImage *, QImage *)));
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

	// Independente de estar rodando ou nao
	// seek no frame 0
	vdo = currentProject->getVideo();

	vdo->seekFrame(0);

	if (vdo_player->isRunning())
		vdo_player->terminate();

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
	vdo_player->mutex.lock();

	Video *vdo = 0;
	vdo = currentProject->getVideo();

	setVideoTime(vdo->getCurrentPosition(), vdo->getFPS());

	QPixmap pix_image = QPixmap::fromImage(*image);

	ui.videoLabel->setScaledContents(true);
	ui.videoLabel->setPixmap(pix_image);

	updateTimeline();

	vdo_player->mutex.unlock();
}

void mfit::updateHist(QImage *image)
{
	vdo_player->mutex.lock();

	QPixmap pix_image = QPixmap::fromImage(*image);

	ui.histogramLabel->setScaledContents(true);
	ui.histogramLabel->setPixmap(pix_image);

	vdo_player->mutex.unlock();
}

void mfit::updateVideoPlayer(QImage *image, int type)
{
	// Atualiza o VideoPlayer
	if (type == 0)
	{
		updateVideoPlayer(image);
	}
	else // Atualiza o Histogram
	{
		updateHist(image);
	}

	return;

}

void mfit::updateVideoPlayer(QImage *image, QImage *imageHist)
{

	if (image != 0x0)
		this->updateVideoPlayer(image);

	if (imageHist != 0x0)
		this->updateHist(imageHist);

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

void mfit::createTimeline(void)
{
	Video *vdo = 0x0;
	vdo = currentProject->getVideo();

	int iter_i = 0x0;
	Frame *frame = 0x0;
	Frame *frameResized = 0x0;
	int i = 0;

	// Timeline sera 10x menor que a imagem
	int width =  SIZE_FRAME_TIMELINE;
	int height = SIZE_FRAME_TIMELINE;

	// Vamos pegar de 1 em 1 segundo
	int sec_i = SIZE_SEC_FRAME;

	iter_i = cvRound(sec_i * vdo->getFPS());

	vdo->seekFrame(0);
	
	frame = vdo->getNextFrame();	
	vdo_player->frameTimeline = frame->resize(width, height);

	delete frame;

	for (i = iter_i ; i < vdo->getFramesTotal() ; i += iter_i)
	{
		vdo->seekFrame(i);

		frame = vdo->getNextFrame();
		frameResized = frame->resize(width, height);

		if (i+iter_i >= vdo->getFramesTotal())
		{
			// O ultimo frame tem que ser proporcional a quantidade de 
			// frames restantes no video
			delete frameResized;
			int posFrame;

			posFrame = iter_i*(cvRound(vdo->getFramesTotal())-i) / SIZE_FRAME_TIMELINE;

			frameResized = frame->resize(posFrame, height);
		}

		*vdo_player->frameTimeline += *frameResized;

		delete frame;
		delete frameResized;
	}

	this->setTimeline(vdo_player->frameTimeline);

	vdo->seekFrame(0);
}


void mfit::setTimeline(Frame *frameTimeline)
{
	QImage *image;

	// Converte a imagem
		image = frameTimeline->IplImageToQImage(&vdo_player->timelineData, &vdo_player->timelineWidth, &vdo_player->timelineHeight);

	QPixmap pix_image = QPixmap::fromImage(*image);

	ui.histogramLabel->setScaledContents(true);
	ui.timelineLabel->setPixmap(pix_image);

	delete image;

}

void mfit::updateTimeline()
{
	Video *vdo = 0x0;
	double pos = 0x0;
	int line_point = 0x0;

	vdo = currentProject->getVideo();
	pos = vdo->getCurrentPosition();

	// A formula para saber em que ponto plotar o indicador �:
	// SIZE_FRAME_TIMELINE  ---- SIZE_SEC_FRAME*vdo->getFPS()
	// pos (ponto timeline) ---- pos (ponto no video)
	line_point  = (SIZE_FRAME_TIMELINE*cvRound(pos)) / (SIZE_SEC_FRAME*cvRound(vdo->getFPS()));

	if (vdo_player->frameTimelineEdited != 0x0)
		delete vdo_player->frameTimelineEdited;

	vdo_player->frameTimelineEdited = new Frame(vdo_player->frameTimeline);

	CvPoint p1 = {line_point,0};
	CvPoint p2 = {line_point,75};

	cvLine(vdo_player->frameTimelineEdited->data, p1, p2, cvScalar(0,0,0), 1);

	setTimeline(vdo_player->frameTimelineEdited);
}

/************************************************************************
 * Trata o evento gerado pelo bot�o para detec��o de todas as transi��es
 *************************************************************************
 * param (E): N�o h�
 * return : n�o h�
 *************************************************************************
 * Hist�rico
 * 06/10/08 - Thiago Mizutani
 * Cria��o.
 ************************************************************************/

void mfit::on_actionAllTransitions_triggered()
{

	Video* vdo = 0x0;
	DetectTransitions* DT = new DetectTransitions();

	vdo = currentProject->getVideo();

	DT->detectTransitions(vdo, &currentProject->transitionList);

	delete DT;

}

/************************************************************************
 * Trata o evento gerado pelo bot�o para detec��o de todos os cortes
 *************************************************************************
 * param (E): N�o h�
 * return : n�o h�
 *************************************************************************
 * Hist�rico
 * 06/10/08 - Thiago Mizutani
 * Cria��o.
 ************************************************************************/

void mfit::on_actionOnlyCuts_triggered()
{

	Video* vdo = 0x0;
	Cut* DTC = new Cut();

	vdo = currentProject->getVideo();

	DTC->detectTransitions(vdo, &currentProject->transitionList);

	delete DTC;

}

/************************************************************************
 * Trata o evento gerado pelo bot�o para detec��o de todos os fades
 *************************************************************************
 * param (E): N�o h�
 * return : n�o h�
 *************************************************************************
 * Hist�rico
 * 06/10/08 - Thiago Mizutani
 * Cria��o.
 ************************************************************************/

void mfit::on_actionAllFades_triggered()
{

	Video* vdo = 0x0;
	Fade* DTF = new Fade();

	vdo = currentProject->getVideo();

	DTF->detectTransitions(vdo, &currentProject->transitionList);

	delete DTF;

}

/*************************************************************************
 * Trata o evento gerado pelo bot�o para detec��o de todos os dissolve
 *************************************************************************
 * param (E): N�o h�
 * return : n�o h�
 *************************************************************************
 * Hist�rico
 * 06/10/08 - Thiago Mizutani
 * Cria��o.
 ************************************************************************/

void mfit::on_actionOnlyDissolve_triggered()
{

	Video* vdo = 0x0;
	Dissolve* DTD = new Dissolve();

	vdo = currentProject->getVideo();

//	DTD->detectTransitions(vdo, currentProject->transitionList);

	delete DTD;

}


