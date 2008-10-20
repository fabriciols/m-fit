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
* Hist�rico
* 19/10/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/

void mfit::addRecentFile(QString fileName)
{
	QSettings settings("MFIT", "MFIT");
	QStringList files = settings.value("recentFileList").toStringList();
	files.removeAll(fileName); // Remove todos os arquivos recentes com o msm nome
	files.prepend(fileName);
	// Retira os ultimos at� que sobrem somente 4.
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
* Hist�rico
* 19/10/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/

void mfit::updateRecentFilesAct()
{
	QSettings settings("MFIT", "MFIT");
	QStringList files = settings.value("recentFileList").toStringList();

	// Verifico o que � menor. O tamanho da lista de arquivos ou o m�ximo permitido.
	int numRecentFiles = qMin(files.size(), (int)MAX_RECENT_FILES);

	// Adiciono um separador.
	ui.menuFile->addSeparator();

	for (int i = 0; i < numRecentFiles; ++i)
	{
		// Monto a string com o n�mero (ordem de abertura) e o nome do arquivo
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
* Hist�rico
* 19/10/08 - Thiago Mizutani
* Inserindo as actions no menu. Por isso n�o estavam aparecendo. b�sico..
* 19/10/08 - Fabricio Lopes de Souza
* Cria��o.
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
	
	// Pego a extens�o do arquivo para verificar se � um projeto ou um v�deo.
	strcpy(extension_cy, &fileName_cy[strlen(fileName_cy)-3]);	

	// Se for um video, uso a funcao de abrir video
	if (!strcmp(extension_cy,"AVI") || !strcmp(extension_cy,"avi"))
	{
		currentProject->openVideo(fileName_cy);
		enableControls();
			
		// Ao carregar o v�deo, devo perguntar ao usu�rio se ele quer fazer a detec��o.
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
 * Tratar o evento do bot�o PLAY. Inicia a thread do video player
 *************************************************************************
 * param (E): Nenhum
 ************************************************************************
 * Hist�rico
 * 13/10/08 - Thiago Mizutani
 * Verifica se h� v�deo, se n�o houver mostra alerta
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/
void mfit::on_playButton_clicked()
{
	// Somente para controle se h� ou n�o v�deo carregado. depois deleta o obj
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
 * Tratar o evento do bot�o PAUSE. Se a thread estiver rodando, mata ela
 *************************************************************************
 * param (E): Nenhum
 ************************************************************************
 * Hist�rico
 * 13/10/08 - Thiago Mizutani
 * Verifica se h� v�deo, se n�o houver mostra alerta
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/
void mfit::on_pauseButton_clicked()
{
	// Somente para controle se h� ou n�o v�deo carregado. depois deleta o obj
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
 * Tratar o evento do bot�o STOP. Se a thread estiver rodando, mata ela,
 * e posiciona o ponteiro do video para seu inicio.
 *************************************************************************
 * param (E): Nenhum
 ************************************************************************
 * Hist�rico
 * 13/10/08 - Thiago Mizutani
 * Verifica se h� v�deo, se n�o houver mostra alerta
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
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
 * Tratar o evento do bot�o OpenProject.
 * TODO: Ler o XML e subir os membros da estrutura Project.
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Hist�rico
 * 10/10/08 - Ivan Shiguenori Machida
 * Cria��o.
 ************************************************************************/
void mfit::on_actionOpenProject_triggered()
{
	// Segundo  parametro - Mensagem que ira aparecer no topo da imagem
	// Terceiro parametro - Diretorio inicial
	// Quarto   parametro - Mensagem q ira aparecer la embaixo, e as extens�es suportadas

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

		enableControls();

		if (askDetection()) // Gero a box perguntando se deseja detectar as transi��es
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
 * Tratar o evento do bot�o Save.
 * TODO: Nao faz porra nenhuma pq nao temos o ParserXML
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Hist�rico
 * 10/10/08 - Ivan Shiguenori Machida
 * Cria��o.
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

/************************************************************************
 * Printa a imagem da timeline em sua label.
 *************************************************************************
 * param (E): N�o h�
 *************************************************************************
 * Hist�rico
 * 09/10/08 - Thiago Mizutani 
 * Revis�o de c�digo. E inser��o de coment�rios
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/

void mfit::setTimeline(Frame *frameTimeline)
{
	QImage *image;

	// Converte a imagem
	image = frameTimeline->IplImageToQImage(&vdo_player->timelineData, &vdo_player->timelineWidth, &vdo_player->timelineHeight);
	//image = frameTimeline->IplImageToQImage();

	QPixmap pix_image = QPixmap::fromImage(*image);

	ui.timelineLabel->setScaledContents(false); // N�o permite que a imagem sofra distor��es conforme o tamanho da janela
	ui.timelineLabel->setPixmap(pix_image); // Pinta a timeline na labelTimeline

	delete image;

}

/*************************************************************************
 * Atualiza a posi��o do cursor da timeline conforme a passagem do v�deo
 *************************************************************************
 * param (E): N�o h�
 *************************************************************************
 * Hist�rico
 * 09/10/08 - Thiago Mizutani 
 * Revis�o de c�digo. E inser��o de coment�rios
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/

void mfit::updateTimeline()
{
	Video *vdo = 0x0;
	long pos = 0x0;
	int line_point = 0x0;

	vdo = currentProject->getVideo();
	pos = (long)vdo->getCurrentPosition();

	// A formula para saber em que ponto plotar o indicador �:
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
 * Trata o evento gerado pelo bot�o para detec��o de todas as transi��es
 *************************************************************************
 * param (E): N�o h�
 * return : n�o h�
 *************************************************************************
 * Hist�rico
 * 13/10/08 - Thiago Mizutani
 * Verifica se h� v�deo, se n�o houver mostra alerta
 * 06/10/08 - Thiago Mizutani
 * Cria��o.
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
 * Trata o evento gerado pelo bot�o para detec��o de todos os cortes
 *************************************************************************
 * param (E): N�o h�
 * return : n�o h�
 *************************************************************************
 * Hist�rico
 * 13/10/08 - Thiago Mizutani
 * Verifica se h� v�deo, se n�o houver mostra alerta
 * 06/10/08 - Thiago Mizutani
 * Cria��o.
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
 * Trata o evento gerado pelo bot�o para detec��o de todos os fades
 *************************************************************************
 * param (E): N�o h�
 * return : n�o h�
 *************************************************************************
 * Hist�rico
 * 13/10/08 - Thiago Mizutani
 * Verifica se h� v�deo, se n�o houver mostra alerta
 * 06/10/08 - Thiago Mizutani
 * Cria��o.
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
 * Trata o evento gerado pelo bot�o para detec��o de todos os dissolve
 *************************************************************************
 * param (E): N�o h�
 **************************************************************************
 * return : n�o h�
 *************************************************************************
 * Hist�rico
 * 13/10/08 - Thiago Mizutani
 * Verifica se h� v�deo, se n�o houver mostra alerta
 * 06/10/08 - Thiago Mizutani
 * Cria��o.
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

void mfit::insertTransitionsTree(Transition* transition, long id_l)
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

	long posTransition_l = 0;
	long posTimeline_l = 0;

	// Pega o ponto da transicao
	posTransition_l = transition->getPosTransition();

	// A formula para saber em que ponto plotar o indicador �:
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

int mfit::askDetection()
{
	int reply = 0; // Resposta do usu�rio

	// Crio uma box com o �cone "?"
	reply = QMessageBox::question(
			this,
			tr("MFIT"),
			tr("<p><b>Detectar todas as transi��es agora?</b>" \
				"<p>Este processo poder� levar alguns minutos."),
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::No
			);

	// Se clickei em detectNow, chamo a detec��o de transi��es
	if (reply == QMessageBox::Yes)
		return (TRUE);
	else
		return (FALSE);

}

/**************************************************************************
 * Fun��o que mostra uma mensagem de alerta caso o usu�io tente fazer 
 * qualquer opera��o que exija v�deo e o v�deo n�o estiver carregado.
 **************************************************************************
 * param (E): Nenhum
 **************************************************************************
 * return : N�o h�
 **************************************************************************
 * Hist�rico
 * 13/10/08 - Thiago Mizutani
 * Cria��o.
 *************************************************************************/

void mfit::alertUser()
{
	QMessageBox::critical(
			this,
			tr("MFIT - ERROR"),
			tr("<p><b>N�o � poss�vel fazer a detec��o de transi��es!</b>" \
				"<p>Carregue um v�deo antes de executar esta opera��o!"),
			QMessageBox::Ok
			);
}

/**************************************************************************
 * Habilita todos os bot�es de controle quando carregar o v�deo.
 **************************************************************************
 * param (E): Nenhum
 **************************************************************************
 * return : N�o h�
 **************************************************************************
 * Hist�rico
 * 13/10/08 - Thiago Mizutani
 * Cria��o.
 *************************************************************************/

void mfit::enableControls()
{
	// Controles gerais
	this->ui.actionSave->setEnabled(true);
	this->ui.actionSaveAs->setEnabled(true);

	// Controles do player (action = menu e toolbar, button = bot�es do player)
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

	// Controles de detec��o
	this->ui.actionAllTransitions->setEnabled(true);
	this->ui.actionOnlyCuts->setEnabled(true);
	this->ui.actionAllFades->setEnabled(true);
	this->ui.actionOnlyDissolve->setEnabled(true);

}

/**************************************************************************
 * Trata evento do click sobre o menu
 **************************************************************************
 * param (E): N�o h�.
 **************************************************************************
 * return : N�o h�.
 **************************************************************************
 * Hist�rico
 * 18/10/08 - Thiago Mizutani
 * Cria��o.
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
 * Hist�rico
 * 14/10/08 - Fabricio Lopes de Souza
 * Cria��o.
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
 * Abre a janela de configura��es para detec��o de transi��es.
 **************************************************************************
 * param (E): N�o h�.
 **************************************************************************
 * return : N�o h�.
 **************************************************************************
 * Hist�rico
 * 18/10/08 - Thiago Mizutani
 * Cria��o.
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
 * Hist�rico
 * 14/10/08 - Fabricio Lopes de Souza
 * Cria��o.
 *************************************************************************/

char *mfit::QStringToChar(QString string, char* string_cy)
{
	sprintf(string_cy, "%s",string.toAscii().data());

	return string_cy;
}

/**************************************************************************
 * Tra�a uma linha acima da timeline identificando a localiza��o da tomada
 **************************************************************************
 * param (E): unsigned int transitionID - Transi��o selecionada na lista.
 * param (E): int clean - flag que indica se deve apagar o tra�o ou n�o.
 **************************************************************************
 * return : N�o h�.
 **************************************************************************
 * Hist�rico
 * 14/10/08 - Fabricio Lopes de Souza
 * Cria��o.
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
