#include "../ui_Render.h"

#include "QWidget.h"

#include "../include/Render.h"
#include "../include/RenderThread.h"
#include "../include/Interface.h"
#include "../include/Effect.h"
#include "../include/Color.h"
#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/TransitionEdit.h"
#include "../include/Project.h"
#include "../include/VideoPlayer.h"

#include "../include/Log.h"

extern Project *currentProject;
extern VideoPlayer *vdo_player;

/************************************************************************
 * Construtor da janela de Renderizacao
 *************************************************************************
 * param (E): QDialog -> referencia da janela que criou o objeto
 ************************************************************************
 * Histórico
 * 20/11/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
Render::Render(QDialog *parent) : QDialog(parent)
{
	ui.setupUi(this);

}

/************************************************************************
 * Seta a porcentagem atual do processo
 *************************************************************************
 * param (E): int percent - porcentagem
 ************************************************************************
 * Histórico
 * 20/11/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
void Render::setProgress(int percent)
{
	renderThread->mutex.lock();

	ui.progressBar->setValue (percent);

	if (percent == 100)
	{
		// Libera o novo video
		cvReleaseVideoWriter(&videoWriter);

		QMessageBox::information(
				this,
				tr("Video Renderizado"),
				tr("Fim da renderização"),
				QMessageBox::Ok
				);

		QDialog::close();

	}

	renderThread->mutex.unlock();
}

/************************************************************************
 * Inicia a thread de renderização
 *************************************************************************
 * param (E): RenderThread - ponteiro para a thread
 ************************************************************************
 * Histórico
 * 20/11/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
void Render::startRenderThread(RenderThread *render)
{
	Video *vdo;

	renderThread = render;

	// Atualiza a imagem do player assim que receber um sinal da thread
	connect(renderThread, SIGNAL(setProgressThread(int)),
			this, SLOT(setProgress(int)));

	qRegisterMetaType<Frame*>("Frame");

	connect(renderThread, SIGNAL(writeFrameThread(Frame*)),
			this, SLOT(writeFrame(Frame*)));

	vdo = currentProject->getVideo();

	// Abre o writer do video
	videoWriter = cvCreateVideoWriter(renderThread->filename_cy, // Nome do arquivo
			CV_FOURCC('D', 'I', 'B', ' '), // Codec
			(int)vdo->getFPS(), // FPS
			cvSize(cvRound(vdo->getFramesWidth()), cvRound(vdo->getFramesHeight())), // Tamanho
			1); // Is color ?

	renderThread->start();
}

/************************************************************************
 * Escreve o frame na estrutura do video
 *************************************************************************
 * param (E): Frame* - frame a ser escrito
 ************************************************************************
 * Histórico
 * 20/11/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
void Render::writeFrame(Frame *frame)
{

	renderThread->mutex.lock();

	int ret_i;

	ret_i = cvWriteFrame(videoWriter, frame->data);

	delete frame;

	renderThread->mutex.unlock();
}
