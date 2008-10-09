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
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
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
	if (!vdo_player->isRunning())
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

	// Independente de estar rodando ou nao
	// seek no frame 0
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
	QList<QTreeWidgetItem *> itens;

	// Adiciona as colunas
	item->setText(0, param_cy);
	item->setText(1, value_cy);

	itens.append(item);

	this->ui.videoPropertiesTree->insertTopLevelItems(0, itens);
}

/************************************************************************
 * Printa um Frame no videoLabel (Video Player)
 *************************************************************************
 * param (E): Frame *frame -> Frame a ser impresso no video player
 *************************************************************************
 * Histórico
 * 29/09/08 - Fabricio Lopes de Souza
 * Criação.
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
 * Histórico
 * 29/09/08 - Fabricio Lopes de Souza
 * Criação.
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
	Frame *frameHeader = 0x0;
	IplImage *imgHeader = 0x0;
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

	// Coloca o frame acima da timeline

	imgHeader = Frame::imgAlloc(
			cvSize(vdo_player->frameTimeline->getWidth(), vdo_player->frameTimeline->getHeight() / 4 ),
			vdo_player->frameTimeline->data->depth,
			vdo_player->frameTimeline->data->nChannels);

	frameHeader = new Frame(imgHeader);

	Frame::imgDealloc(imgHeader);

	cvFillImage(frameHeader->data, 0xF4F4F4); // Preenche da cor de fundo das janelas

	frameResized = vdo_player->frameTimeline->verticalCat(frameHeader);

	delete vdo_player->frameTimeline;

	vdo_player->frameTimeline = frameResized;

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

	frameTimeline->write("timeline.jpg");

	delete image;

}

void mfit::updateTimeline()
{
	Video *vdo = 0x0;
	double pos = 0x0;
	int line_point = 0x0;

	vdo = currentProject->getVideo();
	pos = vdo->getCurrentPosition();

	// A formula para saber em que ponto plotar o indicador é:
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
 * Trata o evento gerado pelo botão para detecção de todas as transições
 *************************************************************************
 * param (E): Não há
 * return : não há
 *************************************************************************
 * Histórico
 * 06/10/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

void mfit::on_actionAllTransitions_triggered()
{

	Video* vdo = 0x0;
	DetectTransitions* DT = new DetectTransitions();

	vdo = currentProject->getVideo();

	DT->detectTransitions(vdo, &currentProject->transitionList);

	delete DT;

	updateTransitions();

}

/************************************************************************
 * Trata o evento gerado pelo botão para detecção de todos os cortes
 *************************************************************************
 * param (E): Não há
 * return : não há
 *************************************************************************
 * Histórico
 * 06/10/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

void mfit::on_actionOnlyCuts_triggered()
{

	Video* vdo = 0x0;
	Cut* DTC = new Cut();

	vdo = currentProject->getVideo();

	DTC->detectTransitions(vdo, &currentProject->transitionList);

	delete DTC;

	updateTransitions();

}

/************************************************************************
 * Trata o evento gerado pelo botão para detecção de todos os fades
 *************************************************************************
 * param (E): Não há
 * return : não há
 *************************************************************************
 * Histórico
 * 06/10/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

void mfit::on_actionAllFades_triggered()
{

	Video* vdo = 0x0;
	Fade* DTF = new Fade();

	vdo = currentProject->getVideo();

	DTF->detectTransitions(vdo, &currentProject->transitionList);

	delete DTF;

	updateTransitions();

}

/*************************************************************************
 * Trata o evento gerado pelo botão para detecção de todos os dissolve
 *************************************************************************
 * param (E): Não há
 * return : não há
 *************************************************************************
 * Histórico
 * 06/10/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

void mfit::on_actionOnlyDissolve_triggered()
{

	Video* vdo = 0x0;
	Dissolve* DTD = new Dissolve();

	vdo = currentProject->getVideo();

	DTD->detectTransitions(vdo, &currentProject->transitionList);

	delete DTD;

	updateTransitions();

}

/**************************************************************************
 * Adiciona uma transição na transitionsTree
 **************************************************************************
 * param (E): Transition* transition -> transição a ser inserida na árovore 
 **************************************************************************
 * return : Não há
 **************************************************************************
 * Histórico
 * 08/10/08 - Thiago Mizutani
 * Criação.
 *************************************************************************/

void mfit::insertTransitionsTree(Transition* transition)
{
	// Cria o item referente a Tree
	QTreeWidgetItem *item = new QTreeWidgetItem(this->ui.transitionsTree);

	// Cria a lista de items
	QList<QTreeWidgetItem *> itens;

	int type_i = transition->getType(); // Tipo da transição
	long posTransition_l = transition->getPosTransition(); // Posição da transição detectada pelo sistema
	long posUserTransition_l = transition->getPosUserTransition(); // Posição da transição detectada

	char posTransition_cy[10];
	char posUserTransition_cy[10];
	
	// Adiciona as colunas
	switch(type_i)
	{
		case TRANSITION_CUT:
			item->setText(0, "Cut");
			break;
		case TRANSITION_FADEIN:
			item->setText(0, "Fade-In");
			break;
		case TRANSITION_FADEOUT:
			item->setText(0, "Fade-Out");
			break;
		case TRANSITION_DISSOLVE:
			item->setText(0, "Dissolve");
			break;
		default:
			break;
	}
	
	sprintf(posTransition_cy, "%ld", posTransition_l);
	sprintf(posUserTransition_cy, "%ld", posUserTransition_l);

	item->setText(1, posTransition_cy);
	item->setText(2, posUserTransition_cy);

	itens.append(item);

	this->ui.transitionsTree->insertTopLevelItems(0, itens);
}

/**************************************************************************
 * Marca na timeline os pontos onde ocorrem as transições
 **************************************************************************
 * param (E): Transition* transition -> Transição a ser marcada na timeline 
 **************************************************************************
 * return : Não há
 **************************************************************************
 * Histórico
 * 08/10/08 - Thiago Mizutani
 * Criação.
 *************************************************************************/
void mfit::insertTransitionsTimeline(Transition* transition)
{

	Video* vdo = currentProject->getVideo();

	long posTransition_l = 0;
	long posTimeline_l = 0;

	// Pega o ponto da transicao
	posTransition_l = transition->getPosTransition();
	
	// A formula para saber em que ponto plotar o indicador é:
	// SIZE_FRAME_TIMELINE  ---- SIZE_SEC_FRAME*vdo->getFPS()
	// pos (ponto timeline) ---- pos (ponto no video)
	
	posTimeline_l = (SIZE_FRAME_TIMELINE*cvRound(posTransition_l)) / (SIZE_SEC_FRAME*cvRound(vdo->getFPS()));

	CvPoint p1 = {posTimeline_l,0};
	CvPoint p2 = {posTimeline_l,SIZE_FRAME_TIMELINE+10};

	cvLine(vdo_player->frameTimelineEdited->data, p1, p2, cvScalar(255,0,0), 2);

	setTimeline(vdo_player->frameTimelineEdited);
	
}

/*************************************************************************
 * Atualiza a lista de transições e marca pontos de transição na timeline
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * return : Não há
 *************************************************************************
 * Histórico
 * 08/10/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

void mfit::updateTransitions()
{
	unsigned int i = 0;

	clearTransitionsTree();

	for (i = 0 ; i < currentProject->transitionList.size() ; i ++)
	{
		insertTransitionsTree(&currentProject->transitionList.at(i));
		insertTransitionsTimeline(&currentProject->transitionList.at(i));
	}
}

/*************************************************************************
 * Limpa a lista de transições - transitionsTree
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * return : Não há
 *************************************************************************
 * Histórico
 * 09/10/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

void mfit::clearTransitionsTree()
{
	Log::writeLog("%s :: clear transitionsTree", __FUNCTION__); 
	this->ui.transitionsTree->clear();
}
