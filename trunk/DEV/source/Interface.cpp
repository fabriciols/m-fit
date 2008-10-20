#include "../ui_mfit.h" 
#include "cv.h"
#include "highgui.h"

#include <algorithm>

#include "QWidget.h"
#include "QDialog.h"
#include <QImage>
#include <QPainter>
#include <QtGui>
#include <QMessageBox>
#include <QPushButton>

#include "../include/DetectConfig.h"

#include "../include/Interface.h"
#include "../include/Effect.h"
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

	memset(recentFiles, '\0', sizeof(recentFiles));

	qRegisterMetaType<Frame>("Frame");

	// Atualiza a imagem do player assim que receber um sinal da thread
	connect(vdo_player, SIGNAL(setNewFrame(QImage *)),
			this, SLOT(updatePlayer(QImage *)));
	
	// Atualiza a imagem do histograma assim que receber um sinal da thread
	connect(vdo_player, SIGNAL(setHistogram(QImage *)),
			this, SLOT(updateHist(QImage *)));

	createRecentFilesActions();
	updateRecentFilesAct();

	// Adiciono o EXIT no final de tudo.
	ui.menuFile->addAction(ui.actionExit);
}

/************************************************************************
* Adiciona um item na lista de arquivos recentes
*************************************************************************
* param (E): QString fileName -> nome do arquivo aberto 
*************************************************************************
* Histórico
* 19/10/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/

void mfit::addRecentFile(QString fileName)
{
	QSettings settings("MFIT", "MFIT");
	QStringList files = settings.value("recentFileList").toStringList();
	files.removeAll(fileName); // Remove todos os arquivos recentes com o msm nome
	files.prepend(fileName);
	// Retira os ultimos até que sobrem somente 4.
	while (files.size() > MAX_RECENT_FILES)
	{
		files.removeLast();
	}

	settings.setValue("recentFileList", files);

}

/************************************************************************
* Atualiza a lista de arquivos recentes
*************************************************************************
* param (E): Nao ha
*************************************************************************
* Histórico
* 19/10/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/

void mfit::updateRecentFilesAct()
{
	QSettings settings("MFIT", "MFIT");
	QStringList files = settings.value("recentFileList").toStringList();

	// Verifico o que é menor. O tamanho da lista de arquivos ou o máximo permitido.
	int numRecentFiles = qMin(files.size(), (int)MAX_RECENT_FILES);

	// Adiciono um separador.
	ui.menuFile->addSeparator();

	for (int i = 0; i < numRecentFiles; ++i)
	{
		// Monto a string com o número (ordem de abertura) e o nome do arquivo
		QString text = tr("&%1 %2").arg(i + 1).arg(QFileInfo(files[i]).fileName());
		recentFiles[i]->setText(text);
		recentFiles[i]->setData(files[i]);
		recentFiles[i]->setVisible(true);
	}

	// Escondo aqueles que forma acima do 4 arquivo aberto recentemente
	for (int j = numRecentFiles; j < MAX_RECENT_FILES ; ++j)
		recentFiles[j]->setVisible(false);
}

/************************************************************************
* Cria a lista de recent Files
*************************************************************************
* param (E): Nao ha
*************************************************************************
* Histórico
* 19/10/08 - Thiago Mizutani
* Inserindo as actions no menu. Por isso não estavam aparecendo. básico..
* 19/10/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
void mfit::createRecentFilesActions()
{
	for (int i = 0; i < MAX_RECENT_FILES ; ++i)
	{
		recentFiles[i] = new QAction(this);
		recentFiles[i]->setVisible(false);
		ui.menuFile->addAction(recentFiles[i]);
		connect(recentFiles[i], SIGNAL(triggered()),
				this, SLOT(openRecentFile()));
	}
}

void mfit::openRecentFile()
{
	char fileName_cy[100];
	char extension_cy[4];

	memset(fileName_cy,'\0',100);

	QAction *action = qobject_cast<QAction *>(sender());

	if (action)
	{
		strcpy(fileName_cy,action->data().toString());
	}
	else
	{
		return;
	}
	
	// Pego a extensão do arquivo para verificar se é um projeto ou um vídeo.
	strcpy(extension_cy, &fileName_cy[strlen(fileName_cy)-3]);	

	// Se for um video, uso a funcao de abrir video
	if (!strcmp(extension_cy,"AVI") || !strcmp(extension_cy,"avi"))
	{
		currentProject->openVideo(fileName_cy);
		enableControls();
			
		// Ao carregar o vídeo, devo perguntar ao usuário se ele quer fazer a detecção.
		if( askDetection() )
		{
			DetectTransitions* DT = new DetectTransitions();
			DT->detectTransitions(currentProject->getVideo(), &currentProject->transitionList);

			updateTransitions();

			delete DT;
		}
	}
	else // Senao, abro um projeto
	{
		currentProject->openProject(fileName_cy);
		updateTransitions();
		enableControls();
	}
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
		enableControls();
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
	//line_point  = (SIZE_FRAME_TIMELINE*cvRound(pos)) / (SIZE_SEC_FRAME*cvRound(vdo->getFPS()));
	line_point = currentProject->FrameToTimelinePos(pos);

	if (vdo_player->frameTimelineEdited != 0x0)
		delete vdo_player->frameTimelineEdited;

	CvPoint p1 = {line_point,0};
	CvPoint p2 = {line_point,SIZE_FRAME_TIMELINE};

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
 **************************************************************************
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
	sprintf(id_cy, "%2.2ld", id_l);

	// Primeira Coluna - ID
	item->setText(0, id_cy);

	// Segunda Coluna - NOME
	switch(type_i)
	{
		case TRANSITION_VIDEOSTART:
			item->setText(1, "Inicio do Video");
			break;
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

	long posTransition_l = 0;
	long posTimeline_l = 0;

	// Pega o ponto da transicao
	posTransition_l = transition->getPosTransition();

	// A formula para saber em que ponto plotar o indicador é:
	// SIZE_FRAME_TIMELINE  ---- SIZE_SEC_FRAME*vdo->getFPS()
	// pos (ponto timeline) ---- pos (ponto no video)

	//posTimeline_l = (SIZE_FRAME_TIMELINE*cvRound(posTransition_l)) / (SIZE_SEC_FRAME*cvRound(vdo->getFPS()));
	posTimeline_l = currentProject->FrameToTimelinePos(posTransition_l);

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

	currentProject->sortTransitionList();

	clearTransitionsTree();

	Log::writeLog("%s :: updating Transitions", __FUNCTION__);

	for (i = 0 ; i < currentProject->transitionList.size() ; i ++)
	{
		insertTransitionsTree(&currentProject->transitionList.at(i), i);
		insertTransitionsTimeline(&currentProject->transitionList.at(i));
	}

	clearTransitionHeader();

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

void mfit::on_transitionsTree_itemSelectionChanged()
{
	QList<QTreeWidgetItem *> itens;
	itens = this->ui.transitionsTree->selectedItems();

	QTreeWidgetItem *item;

	clearTransitionHeader();

	foreach(item, itens)
	{
		updateTransitionHeader(item);
	}

	updateTimeline();
}

void mfit::updateTransitionHeader(QTreeWidgetItem * item)
{
	char str_cy[20];
	long idx_i;
	QString text = item->text(0);

	memset(str_cy, '\0', sizeof(str_cy));

	QStringToChar(text, str_cy);

	idx_i = atoi(str_cy);

	updateTransitionHeader(idx_i);
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

/**************************************************************************
 * Trata evento do click sobre o menu
 **************************************************************************
 * param (E): Não há.
 **************************************************************************
 * return : Não há.
 **************************************************************************
 * Histórico
 * 18/10/08 - Thiago Mizutani
 * Criação.
 *************************************************************************/

void mfit::on_actionDetectConfig_triggered()
{
	showDetectionConfigs();
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

void mfit::on_actionRenderVideo_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(this,
			"Render Video",
			".",
			"Supported Videos (*.avi)");

	if (!fileName.isEmpty())
	{
		char filename_cy[100];

		QStringToChar(fileName, filename_cy);

		currentProject->renderVideo(filename_cy);
	}
}

/**************************************************************************
 * Abre a janela de configurações para detecção de transições.
 **************************************************************************
 * param (E): Não há.
 **************************************************************************
 * return : Não há.
 **************************************************************************
 * Histórico
 * 18/10/08 - Thiago Mizutani
 * Criação.
 *************************************************************************/

void mfit::showDetectionConfigs()
{

	DetectConfig configWindow;

	configWindow.exec();

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

/**************************************************************************
 * Traça uma linha acima da timeline identificando a localização da tomada
 **************************************************************************
 * param (E): unsigned int transitionID - Transição selecionada na lista.
 * param (E): int clean - flag que indica se deve apagar o traço ou não.
 **************************************************************************
 * return : Não há.
 **************************************************************************
 * Histórico
 * 14/10/08 - Fabricio Lopes de Souza
 * Criação.
 *************************************************************************/

void mfit::updateTransitionHeader(unsigned int transitionID, int clean)
{
	long pos_l;
	long posNext_l;

	unsigned int posTimeline;
	unsigned int posTimelineNext;

	CvScalar scalar;

	Transition *transition;
	Transition *transitionNEXT;

	transition = &currentProject->transitionList.at(transitionID);

	pos_l = transition->getPosCurrent();

	if (transitionID == (unsigned int)currentProject->transitionList.size()-1)
	{
		Video *vdo = 0x0;

		vdo = currentProject->getVideo();

		posNext_l = (long)vdo->getFramesTotal();
	}
	else
	{

		transitionNEXT = &currentProject->transitionList.at(transitionID+1);
		posNext_l = transitionNEXT->getPosCurrent();

	}

	// Printa uma linha de pos ate posNext
	posTimeline     = currentProject->FrameToTimelinePos(pos_l);
	posTimelineNext = currentProject->FrameToTimelinePos(posNext_l);

	CvPoint p1 = {posTimeline    ,SIZE_FRAME_TIMELINE+15};
	CvPoint p2 = {posTimelineNext,SIZE_FRAME_TIMELINE+15};

	if (clean == 1)
	{  // Apaga na timelie
		scalar.val[0] = 244;
		scalar.val[1] = 244;
		scalar.val[2] = 244;
		transition->selected = false;
	}
	else
	{  // Imprime na timeline
		scalar.val[0] = 50;
		scalar.val[1] = 30;
		scalar.val[2] = 20;
		transition->selected = true;
	}

	cvLine(vdo_player->frameTimeline->data, p1, p2, scalar, 3);


}

void mfit::clearTransitionHeader()
{
	unsigned int i = 0;
	Transition *transition;
	for (i = 0 ; i < currentProject->transitionList.size() ; i ++)
	{
		transition = &currentProject->transitionList.at(i);

		updateTransitionHeader(i, 1);
	}
}

void mfit::insertEffectTree(Effect *effect)
{
	char frameStart[10];
	char frameEnd[10];

	// Cria o item referente a Tree
	QTreeWidgetItem *item = new QTreeWidgetItem(this->ui.effectsTree);

	// Cria a lista de items
	QList<QTreeWidgetItem *> itens;

	sprintf(frameStart, "%ld", effect->frameStart);
	sprintf(frameEnd  , "%ld", effect->frameEnd);

	// Primeira Coluna - Nome do efeito
	item->setText(0, "Efeito");

	// Segunda coluna - Frame de start
	item->setText(1, frameStart);

	// Terceira Coluna - Frame final
	item->setText(2, frameEnd);

	itens.append(item);

	this->ui.effectsTree->insertTopLevelItems(0, itens);
}

void mfit::on_effectsTree_itemClicked(QTreeWidgetItem * item, int column)
{
	if (column > 0)
	{
		char framepos_cy[10];
		long framepos_l;
		Video *vdo;

		QStringToChar(item->text(column), framepos_cy);

		framepos_l = atol(framepos_cy);

		vdo = currentProject->getVideo();

		if (framepos_l >= vdo->getFramesTotal())
			framepos_l = (long)vdo->getFramesTotal() - 1;

		vdo->seekFrame(framepos_l);

		vdo_player->updatePlayer(vdo->getCurrentFrame());

	}
	else // Se clicou na coluna do nome do efeito
	{
		return;
	}
}

void mfit::updateEffectTree()
{
	unsigned int i = 0;

	effectTreeClear();

	for ( i = 0 ; i < currentProject->effectList.size() ; i++)
	{
		insertEffectTree(currentProject->effectList.at(i));
	}
}

void mfit::effectTreeClear()
{
	this->ui.effectsTree->clear();
}
