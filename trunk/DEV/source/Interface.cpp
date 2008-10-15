#include "../ui_mfit.h" 
#include "cv.h"
#include "highgui.h"

#include "QWidget.h"
#include "QDialog.h"
#include <QImage>
#include <QPainter>
#include <QtGui>
#include <QMessageBox>
#include <QPushButton>

#include "../include/CutConfig.h"

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
* 09/10/08 - Thiago Mizutani
* Agora utilizo um connect para cada coisa. Um para o player outro para
* o histograma.
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
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
* Tratar o evento do botão PLAY. Inicia a thread do video player
*************************************************************************
* param (E): Nenhum
************************************************************************
* Histórico
* 13/10/08 - Thiago Mizutani
* Verifica se há vídeo, se não houver mostra alerta
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
void mfit::on_playButton_clicked()
{
	// Somente para controle se há ou não vídeo carregado. depois deleta o obj
	Video* vdo = currentProject->getVideo();

	if (!vdo)
	{
		alertUser();
		delete vdo;
		return;
	}

	delete vdo;

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
 * 13/10/08 - Thiago Mizutani
 * Verifica se há vídeo, se não houver mostra alerta
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
void mfit::on_pauseButton_clicked()
{
	// Somente para controle se há ou não vídeo carregado. depois deleta o obj
	Video* vdo = currentProject->getVideo();

	if (!vdo)
	{
		alertUser();
		delete vdo;
		return;
	}

	delete vdo;

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
 * 13/10/08 - Thiago Mizutani
 * Verifica se há vídeo, se não houver mostra alerta
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
void mfit::on_stopButton_clicked()
{
	Video *vdo;

	// Independente de estar rodando ou nao
	// seek no frame 0
	vdo = currentProject->getVideo();

	if (!vdo)
	{
		alertUser();
		delete vdo;
		return;
	}

	vdo->seekFrame(0);

	if (vdo_player->isRunning())
		vdo_player->terminate();
	
	delete vdo;

}

/************************************************************************
 * Tratar o evento do botão OpenProject.
 * TODO: Ler o XML e subir os membros da estrutura Project.
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Histórico
 * 10/10/08 - Ivan Shiguenori Machida
 * Criação.
 ************************************************************************/
void mfit::on_actionOpenProject_triggered()
{
	// Segundo  parametro - Mensagem que ira aparecer no topo da imagem
	// Terceiro parametro - Diretorio inicial
	// Quarto   parametro - Mensagem q ira aparecer la embaixo, e as extensões suportadas

	QString fileName = QFileDialog::getOpenFileName(this,
			"Open Project",
			".",
			"MFIT Project File (*.mfit)");

	if (!fileName.isEmpty())
	{
		currentProject->openProject(fileName);
		updateTransitions();
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
 * 09/10/08 - Thiago Mizutani
 * Pergunta para usuário se deseja detectar todas as transições após o
 * carregamento do vídeo.
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

		enableControls();

		if (askDetection()) // Gero a box perguntando se deseja detectar as transições
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
	QString fileName = QFileDialog::getSaveFileName(this,
			"Save Project",
			".",
			"MFIT Project File (*.mfit)");

	if (!fileName.isEmpty())
	{
		return;
	}
}

/************************************************************************
 * Tratar o evento do botão Save.
 * TODO: Nao faz porra nenhuma pq nao temos o ParserXML
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Histórico
 * 10/10/08 - Ivan Shiguenori Machida
 * Criação.
 ************************************************************************/
void mfit::on_actionSave_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(this,
			"Save Project",
			".",
			"MFIT Project File (*.mfit)");

	if (!fileName.isEmpty())
	{
		currentProject->saveProject(fileName);
		
		this->setWindowTitle(fileName.right(fileName.length() - fileName.lastIndexOf("/") - 1));

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
 * Limpa a lista de propriedades do video (videoPropertiesTree)
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
 * Insere uma linha na lista de propriedades do vídeo.
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

	item->setCheckState(0, Qt::Unchecked);

	itens.append(item);

	this->ui.videoPropertiesTree->insertTopLevelItems(0, itens);
}

/************************************************************************
 * Atualiza o frame exibido na label do player
 *************************************************************************
 * param (E): QImage *image -> Imagem a ser impressa no video player
 *************************************************************************
 * Histórico
 * 09/10/08 - Thiago Mizutani
 * Revisão - alterado o nome da função para diminuir o nro de funcoes
 * 29/09/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
void mfit::updatePlayer(QImage *image)
{
	// Trava a thread do video_player
	vdo_player->mutex.lock();

	Video *vdo = currentProject->getVideo();

	setVideoTime(vdo->getCurrentPosition(), vdo->getFPS());

	QPixmap pix_image = QPixmap::fromImage(*image);

	// Permite ajute da imagem em relação ao tamanho da janela
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
 * Histórico
 * 09/10/08 - Thiago Mizutani
 * Revisão - alterado o nome da função para diminuir o nro de funcoes e
 * e inserção dos comentários
 * 29/09/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/

void mfit::updateHist(QImage *hist)
{
	vdo_player->mutex.lock();

	QPixmap pix_image = QPixmap::fromImage(*hist);

	// Permite ajute da imagem em relação ao tamanho da janela
	ui.histogramLabel->setScaledContents(true);
	ui.histogramLabel->setPixmap(pix_image); // Pinta a imagem

	vdo_player->mutex.unlock();
}

/************************************************************************
 * ??????????
 *************************************************************************
 * param (E): QImage *hist-> Novo histograma 
 *************************************************************************
 * Histórico
 * 29/09/08 - Fabricio Lopes de Souza
 * Criação.
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
 * Cria a timeline do vídeo carregado
 *************************************************************************
 * param (E): Não há
 *************************************************************************
 * Histórico
 * 09/10/08 - Thiago Mizutani 
 * Revisão de código. E inserção de comentários
 * 29/09/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/

void mfit::createTimeline(void)
{
	Video *vdo = 0x0;
	vdo = currentProject->getVideo();

	int iter_i = 0x0; // Passo da timeline (qtos frames eu devo pular para plotar o próximo na timeline)
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

/************************************************************************
 * Printa a imagem da timeline em sua label.
 *************************************************************************
 * param (E): Não há
 *************************************************************************
 * Histórico
 * 09/10/08 - Thiago Mizutani 
 * Revisão de código. E inserção de comentários
 * 29/09/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/

void mfit::setTimeline(Frame *frameTimeline)
{
	QImage *image;

	// Converte a imagem
	image = frameTimeline->IplImageToQImage(&vdo_player->timelineData, &vdo_player->timelineWidth, &vdo_player->timelineHeight);
	//image = frameTimeline->IplImageToQImage();

	QPixmap pix_image = QPixmap::fromImage(*image);

	ui.timelineLabel->setScaledContents(false); // Não permite que a imagem sofra distorções conforme o tamanho da janela
	ui.timelineLabel->setPixmap(pix_image); // Pinta a timeline na labelTimeline

	delete image;

}

/*************************************************************************
 * Atualiza a posição do cursor da timeline conforme a passagem do vídeo
 *************************************************************************
 * param (E): Não há
 *************************************************************************
 * Histórico
 * 09/10/08 - Thiago Mizutani 
 * Revisão de código. E inserção de comentários
 * 29/09/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/

void mfit::updateTimeline()
{
	Video *vdo = 0x0;
	long pos = 0x0;
	int line_point = 0x0;

	vdo = currentProject->getVideo();
	pos = (long)vdo->getCurrentPosition();

	// A formula para saber em que ponto plotar o indicador é:
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
 * Trata o evento gerado pelo botão para detecção de todas as transições
 *************************************************************************
 * param (E): Não há
 * return : não há
 *************************************************************************
 * Histórico
 * 13/10/08 - Thiago Mizutani
 * Verifica se há vídeo, se não houver mostra alerta
 * 06/10/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

void mfit::on_actionAllTransitions_triggered()
{

	Video* vdo = 0x0;
	long posFrame = 0x0;
	DetectTransitions* DT;

	vdo = currentProject->getVideo();

	if (vdo == 0x0)
	{
		alertUser();
		return;
	}

	DT = new DetectTransitions();

	posFrame = (long)vdo->getCurrentPosition();

	vdo->seekFrame(0);

	DT->detectTransitions(vdo, &currentProject->transitionList);

	vdo->seekFrame(posFrame);

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
 * 13/10/08 - Thiago Mizutani
 * Verifica se há vídeo, se não houver mostra alerta
 * 06/10/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

void mfit::on_actionOnlyCuts_triggered()
{

	Video* vdo = 0x0;
	long posFrame = 0x0;
	Cut* DTC = 0x0;

	vdo = currentProject->getVideo();

	if (vdo == 0x0)
	{
		alertUser();
		return;
	}

	DTC = new Cut();

	posFrame = (long)vdo->getCurrentPosition();

	vdo->seekFrame(0);

	DTC->detectTransitions(vdo, &currentProject->transitionList);

	vdo->seekFrame(posFrame);

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
 * 13/10/08 - Thiago Mizutani
 * Verifica se há vídeo, se não houver mostra alerta
 * 06/10/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

void mfit::on_actionAllFades_triggered()
{

	Video* vdo = 0x0;
	long posFrame = 0x0;
	Fade* DTF = 0x0;

	vdo = currentProject->getVideo();

	if (vdo == 0x0)
	{
		alertUser();
		return;
	}

	DTF = new Fade();

	posFrame = (long)vdo->getCurrentPosition();

	vdo->seekFrame(0);

	DTF->detectTransitions(vdo, &currentProject->transitionList);

	vdo->seekFrame(posFrame);

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
 * 13/10/08 - Thiago Mizutani
 * Verifica se há vídeo, se não houver mostra alerta
 * 06/10/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

void mfit::on_actionOnlyDissolve_triggered()
{

	Video* vdo = 0x0;
	long posFrame = 0x0;
	Dissolve* DTD = 0x0;

	vdo = currentProject->getVideo();

	if (vdo == 0x0)
	{
		alertUser();
		return;
	}

	DTD = new Dissolve();

	posFrame = (long)vdo->getCurrentPosition();

	vdo->seekFrame(0);

	DTD->detectTransitions(vdo, &currentProject->transitionList);

	vdo->seekFrame(posFrame);

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

void mfit::insertTransitionsTree(Transition* transition, long id_l)
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
	char id_cy[20];

	sprintf(posTransition_cy, "%ld", posTransition_l);
	sprintf(posUserTransition_cy, "%ld", posUserTransition_l);
	sprintf(id_cy, "%ld", id_l);

	// Primeira Coluna - ID
	item->setText(0, id_cy);

	// Segunda Coluna - NOME
	switch(type_i)
	{
		case TRANSITION_CUT:
			item->setText(1, "Cut");
			break;
		case TRANSITION_FADEIN:
			item->setText(1, "Fade-In");
			break;
		case TRANSITION_FADEOUT:
			item->setText(1, "Fade-Out");
			break;
		case TRANSITION_DISSOLVE:
			item->setText(1, "Dissolve");
			break;
		default:
			break;
	}

	// Terceira coluna - POSICAO
	if (posUserTransition_l > 0)
	{
		item->setText(2, posUserTransition_cy);
	}
	else
	{
		item->setText(2, posTransition_cy);
	}

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

	CvPoint p1 = {posTimeline_l,0}; // Ponto inicial
	CvPoint p2 = {posTimeline_l,SIZE_FRAME_TIMELINE+10}; // Ponto final da reta + 10 (pois deve ser maior que a timeline)

	// Sempre trem que ser aplicado ao frameTimeline
	cvLine(vdo_player->frameTimeline->data, p1, p2, cvScalar(255,0,0), 2);
	setTimeline(vdo_player->frameTimeline);

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
	unsigned long i = 0;

	clearTransitionsTree();

	Log::writeLog("%s :: updating Transitions", __FUNCTION__);

	for (i = 0 ; i < currentProject->transitionList.size() ; i ++)
	{
		insertTransitionsTree(&currentProject->transitionList.at(i), i);
		insertTransitionsTimeline(&currentProject->transitionList.at(i));
	}

	updateTimeline();
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

void mfit::on_transitionsTree_itemDoubleClicked( QTreeWidgetItem * item, int column )
{
	QString text = item->text(1);
	char *str_cy;

	str_cy = (char*)malloc(sizeof(text.size()+1));

	memset(str_cy, '\0', sizeof(str_cy));

	QStringToChar(text, str_cy);

}

/**************************************************************************
 * Cria uma caixa de diálogo perguntando ao usuário se este deseja realizar
 * a detecção de transições logo após o carregamento do vídeo.
 **************************************************************************
 * param (E): Nenhum
 **************************************************************************
 * return : Não há
 **************************************************************************
 * Histórico
 * 09/10/08 - Thiago Mizutani
 * Criação.
 *************************************************************************/

int mfit::askDetection()
{
	int reply = 0; // Resposta do usuário

	// Crio uma box com o ícone "?"
	reply = QMessageBox::question(
			this,
			tr("MFIT"),
			tr("<p><b>Detectar todas as transições agora?</b>" \
				"<p>Este processo poderá levar alguns minutos."),
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::No
			);

	// Se clickei em detectNow, chamo a detecção de transições
	if (reply == QMessageBox::Yes)
		return (TRUE);
	else
		return (FALSE);

}

/**************************************************************************
 * Função que mostra uma mensagem de alerta caso o usuáio tente fazer 
 * qualquer operação que exija vídeo e o vídeo não estiver carregado.
 **************************************************************************
 * param (E): Nenhum
 **************************************************************************
 * return : Não há
 **************************************************************************
 * Histórico
 * 13/10/08 - Thiago Mizutani
 * Criação.
 *************************************************************************/

void mfit::alertUser()
{
	QMessageBox::critical(
			this,
			tr("MFIT - ERROR"),
			tr("<p><b>Não é possível fazer a detecção de transições!</b>" \
				"<p>Carregue um vídeo antes de executar esta operação!"),
			QMessageBox::Ok
			);
}

/**************************************************************************
 * Habilita todos os botões de controle quando carregar o vídeo.
 **************************************************************************
 * param (E): Nenhum
 **************************************************************************
 * return : Não há
 **************************************************************************
 * Histórico
 * 13/10/08 - Thiago Mizutani
 * Criação.
 *************************************************************************/

void mfit::enableControls()
{
	// Controles gerais
	this->ui.actionSave->setEnabled(true);
	this->ui.actionSaveAs->setEnabled(true);
	
	// Controles do player (action = menu e toolbar, button = botões do player)
	this->ui.actionPlay->setEnabled(true);
	this->ui.playButton->setEnabled(true);
	
	this->ui.actionPause->setEnabled(true);
	this->ui.pauseButton->setEnabled(true);
	
	this->ui.actionBackward->setEnabled(true);
	this->ui.backButton->setEnabled(true);
	
	this->ui.actionForward->setEnabled(true);
	this->ui.forwardButton->setEnabled(true);
	
	this->ui.actionStop->setEnabled(true);
	this->ui.stopButton->setEnabled(true);

	// Controles de detecção
	this->ui.actionAllTransitions->setEnabled(true);
	this->ui.actionOnlyCuts->setEnabled(true);
	this->ui.actionAllFades->setEnabled(true);
	this->ui.actionOnlyDissolve->setEnabled(true);

}

void mfit::on_actionCut_Settings_triggered()
{
	int newThreshold = 0;

	newThreshold = askNewThreshold();

	if (newThreshold)
		currentProject->setUserThreshold(newThreshold);
}

int mfit::askNewThreshold()
{
	/*
 	bool ok;
	// Por default iremos definir o limiar como 45% da altura do frame 
	// (tal valor foi definido através de testes).
	int defaultThreshold = 45; 
   int userThreshold;

	userThreshold = currentProject->getUserThreshold();

	Log::writeLog("%s, user[%d], default[%d]", __FUNCTION__, userThreshold, defaultThreshold);

	if (userThreshold != 0)
		defaultThreshold = userThreshold;

	Log::writeLog("%s, user[%d], default[%d]", __FUNCTION__, userThreshold, defaultThreshold);

	userThreshold = QInputDialog::getInteger(
							this, 
							tr("MFIT"), // Nome da janela
                     tr("Porcentagem:"), // Texto
							defaultThreshold, // Valor inicial
							0, // Valor minimo
							100, // Valor máximo
							1, // Incremento
							&ok); // Botão de saida

   if (ok && userThreshold)
		return (userThreshold);
	else
		return 0;
*/

	CutConfig cutWindow;

	cutWindow.exec();

	return 0;

}

/**************************************************************************
 * Transforma uma QString em um char*
 **************************************************************************
 * param (E): QString *string    - String do tipo QSTRING
 * param (S): char    *string_cy - String convertida para char*
 **************************************************************************
 * return : char* - String convertida
 **************************************************************************
 * Histórico
 * 14/10/08 - Fabricio Lopes de Souza
 * Criação.
 *************************************************************************/
char *mfit::QStringToChar(QString string, char* string_cy)
{
	sprintf(string_cy, "%s",string.toAscii().data());

	return string_cy;
}
