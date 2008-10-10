#include "../ui_mfit.h" 
#include "cv.h"
#include "highgui.h"

#include "QWidget.h"
#include "QDialog.h"
#include <QImage>
#include <QPainter>
#include <QtGui>
#include <QDialog>
#include <QMessageBox>
#include <QPushButton>

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
* 09/10/08 - Thiago Mizutani
* Agora utilizo um connect para cada coisa. Um para o player outro para
* o histograma.
* 29/09/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
mfit::mfit(QMainWindow *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	qRegisterMetaType<Frame>("Frame");

	// Atualiza a imagem do player assim que receber um sinal da thread
	connect(vdo_player, SIGNAL(setNewFrame(QImage *)),
			this, SLOT(updatePlayer(QImage *)));
	
	// Atualiza a imagem do histograma assim que receber um sinal da thread
	connect(vdo_player, SIGNAL(setHistogram(QImage *)),
			this, SLOT(updateHist(QImage *)));

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
 * 09/10/08 - Thiago Mizutani
 * Pergunta para usu�rio se deseja detectar todas as transi��es ap�s o
 * carregamento do v�deo.
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

		if (askUser()) // Gero a box perguntando se deseja detectar as transi��es
		{  
			DetectTransitions* DT = new DetectTransitions();
			DT->detectTransitions(currentProject->getVideo(), &currentProject->transitionList);

			updateTransitions();

			delete DT;

		}
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
 * Limpa a lista de propriedades do video (videoPropertiesTree)
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
 * Insere uma linha na lista de propriedades do v�deo.
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
	QList<QTreeWidgetItem *> itens;

	// Adiciona as colunas
	item->setText(0, param_cy);
	item->setText(1, value_cy);

	itens.append(item);

	this->ui.videoPropertiesTree->insertTopLevelItems(0, itens);
}

/************************************************************************
 * Atualiza o frame exibido na label do player
 *************************************************************************
 * param (E): QImage *image -> Imagem a ser impressa no video player
 *************************************************************************
 * Hist�rico
 * 09/10/08 - Thiago Mizutani
 * Revis�o - alterado o nome da fun��o para diminuir o nro de funcoes
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/
void mfit::updatePlayer(QImage *image)
{
	// Trava a thread do video_player
	vdo_player->mutex.lock();

	Video *vdo = currentProject->getVideo();

	setVideoTime(vdo->getCurrentPosition(), vdo->getFPS());

	QPixmap pix_image = QPixmap::fromImage(*image);

	// Permite ajute da imagem em rela��o ao tamanho da janela
	ui.videoLabel->setScaledContents(true);
	ui.videoLabel->setPixmap(pix_image); // Pinta a imagem

	updateTimeline();

	vdo_player->mutex.unlock();
}

/************************************************************************
 * Atualiza o histograma da widgetHistogram.
 *************************************************************************
 * param (E): QImage *hist-> Novo histograma 
 *************************************************************************
 * Hist�rico
 * 09/10/08 - Thiago Mizutani
 * Revis�o - alterado o nome da fun��o para diminuir o nro de funcoes e
 * e inser��o dos coment�rios
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/

void mfit::updateHist(QImage *hist)
{
	vdo_player->mutex.lock();

	QPixmap pix_image = QPixmap::fromImage(*hist);

	// Permite ajute da imagem em rela��o ao tamanho da janela
	ui.histogramLabel->setScaledContents(true);
	ui.histogramLabel->setPixmap(pix_image); // Pinta a imagem

	vdo_player->mutex.unlock();
}

/************************************************************************
 * ??????????
 *************************************************************************
 * param (E): QImage *hist-> Novo histograma 
 *************************************************************************
 * Hist�rico
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/

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

/************************************************************************
 * Cria a timeline do v�deo carregado
 *************************************************************************
 * param (E): N�o h�
 *************************************************************************
 * Hist�rico
 * 09/10/08 - Thiago Mizutani 
 * Revis�o de c�digo. E inser��o de coment�rios
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/

void mfit::createTimeline(void)
{
	Video *vdo = 0x0;
	vdo = currentProject->getVideo();

	int iter_i = 0x0; // Passo da timeline (qtos frames eu devo pular para plotar o pr�ximo na timeline)
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

	// Coloca o header da timeline
	imgHeader = Frame::imgAlloc(
			cvSize(vdo_player->frameTimeline->getWidth(), vdo_player->frameTimeline->getHeight() / 4 ),
			vdo_player->frameTimeline->data->depth,
			vdo_player->frameTimeline->data->nChannels);

	frameHeader = new Frame(imgHeader);

	Frame::imgDealloc(imgHeader);

	cvFillImage(frameHeader->data, 0xF4F4F4); // Preenche da cor de fundo das janelas

	frameResized = vdo_player->frameTimeline->verticalCat(frameHeader);

	// Vamos desenha o contorno da timeline

	delete vdo_player->frameTimeline;

	vdo_player->frameTimeline = frameResized;
	vdo_player->frameTimelineOriginal = new Frame(vdo_player->frameTimeline);

	this->setTimeline(vdo_player->frameTimeline);

	vdo->seekFrame(0);
}


void mfit::setTimeline(Frame *frameTimeline)
{
	QImage *image;

	// Converte a imagem
		image = frameTimeline->IplImageToQImage(&vdo_player->timelineData, &vdo_player->timelineWidth, &vdo_player->timelineHeight);

	QPixmap pix_image = QPixmap::fromImage(*image);

	ui.timelineLabel->setScaledContents(false); // N�o permite que a imagem sofra distor��es conforme o tamanho da janela
	ui.timelineLabel->setPixmap(pix_image); // Pinta a timeline na labelTimeline

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

	CvPoint p1 = {line_point,0};
	CvPoint p2 = {line_point,75};

	vdo_player->frameTimelineEdited = new Frame(vdo_player->frameTimeline);

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

	updateTransitions();

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

	updateTransitions();

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

	updateTransitions();

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

	DTD->detectTransitions(vdo, &currentProject->transitionList);

	delete DTD;

	updateTransitions();

}

/**************************************************************************
 * Adiciona uma transi��o na transitionsTree
 **************************************************************************
 * param (E): Transition* transition -> transi��o a ser inserida na �rovore 
 **************************************************************************
 * return : N�o h�
 **************************************************************************
 * Hist�rico
 * 08/10/08 - Thiago Mizutani
 * Cria��o.
 *************************************************************************/

void mfit::insertTransitionsTree(Transition* transition)
{
	// Cria o item referente a Tree
	QTreeWidgetItem *item = new QTreeWidgetItem(this->ui.transitionsTree);

	// Cria a lista de items
	QList<QTreeWidgetItem *> itens;

	int type_i = transition->getType(); // Tipo da transi��o
	long posTransition_l = transition->getPosTransition(); // Posi��o da transi��o detectada pelo sistema
	long posUserTransition_l = transition->getPosUserTransition(); // Posi��o da transi��o detectada

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
 * Marca na timeline os pontos onde ocorrem as transi��es
 **************************************************************************
 * param (E): Transition* transition -> Transi��o a ser marcada na timeline 
 **************************************************************************
 * return : N�o h�
 **************************************************************************
 * Hist�rico
 * 08/10/08 - Thiago Mizutani
 * Cria��o.
 *************************************************************************/
void mfit::insertTransitionsTimeline(Transition* transition)
{

	Video* vdo = currentProject->getVideo();

	long posTransition_l = 0;
	long posTimeline_l = 0;

	// Pega o ponto da transicao
	posTransition_l = transition->getPosTransition();
	
	// A formula para saber em que ponto plotar o indicador �:
	// SIZE_FRAME_TIMELINE  ---- SIZE_SEC_FRAME*vdo->getFPS()
	// pos (ponto timeline) ---- pos (ponto no video)
	
	posTimeline_l = (SIZE_FRAME_TIMELINE*cvRound(posTransition_l)) / (SIZE_SEC_FRAME*cvRound(vdo->getFPS()));

	CvPoint p1 = {posTimeline_l,0}; // Ponto inicial
	CvPoint p2 = {posTimeline_l,SIZE_FRAME_TIMELINE+10}; // Ponto final da reta + 10 (pois deve ser maior que a timeline)

	// Sempre trem que ser aplicado ao frameTimeline
	cvLine(vdo_player->frameTimeline->data, p1, p2, cvScalar(255,0,0), 2);
	setTimeline(vdo_player->frameTimeline);

}

/*************************************************************************
 * Atualiza a lista de transi��es e marca pontos de transi��o na timeline
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * return : N�o h�
 *************************************************************************
 * Hist�rico
 * 08/10/08 - Thiago Mizutani
 * Cria��o.
 ************************************************************************/

void mfit::updateTransitions()
{
	unsigned int i = 0;

	clearTransitionsTree();

	Log::writeLog("%s :: updating Transitions", __FUNCTION__);

	for (i = 0 ; i < currentProject->transitionList.size() ; i ++)
	{
		insertTransitionsTree(&currentProject->transitionList.at(i));
		insertTransitionsTimeline(&currentProject->transitionList.at(i));
	}

	updateTimeline();
}

/*************************************************************************
 * Limpa a lista de transi��es - transitionsTree
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * return : N�o h�
 *************************************************************************
 * Hist�rico
 * 09/10/08 - Thiago Mizutani
 * Cria��o.
 ************************************************************************/

void mfit::clearTransitionsTree()
{
	Log::writeLog("%s :: clear transitionsTree", __FUNCTION__); 
	this->ui.transitionsTree->clear();
}

void mfit::on_transitionsTree_itemDoubleClicked( QTreeWidgetItem * item, int column )
{
	char *pos;
	QString text = item->text(0);

	QByteArray ba = text.toLatin1();
	pos = ba.data(); 
}

/**************************************************************************
 * Cria uma caixa de di�logo perguntando ao usu�rio se este deseja realizar
 * a detec��o de transi��es logo ap�s o carregamento do v�deo.
 **************************************************************************
 * param (E): Nenhum
 **************************************************************************
 * return : N�o h�
 **************************************************************************
 * Hist�rico
 * 09/10/08 - Thiago Mizutani
 * Cria��o.
 *************************************************************************/

int mfit::askUser()
{
	int reply = 0; // Resposta do usu�rio
	
	// Crio uma box com o �cone "?"
	reply = QMessageBox::question(
					this,
					tr("MFIT"),
					tr("<p><b>Detect all transitions now?</b>" \
						"<p>This action can take several minutes."),
					QMessageBox::Yes | QMessageBox::No
			  );

	// Se clickei em detectNow, chamo a detec��o de transi��es
	if (reply == QMessageBox::Yes)
		return (TRUE);
	else
		return (FALSE);

}
