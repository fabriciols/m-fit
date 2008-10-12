#include "../include/VideoPlayer.h"
#include "../include/Histogram.h"
#include "../include/Effect.h"
#include "../include/Color.h"

extern Project *currentProject;
extern mfit *mfit_ui;

/************************************************************************
 * Construtor
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Hist�rico
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/

VideoPlayer::VideoPlayer(void)
{
	// Variaveis internas
	this->imageData = 0x0;
	this->imgWidth  = 0x0;
	this->imgHeight = 0x0;

	this->histData = 0x0;
	this->histWidth  = 0x0;
	this->histHeight = 0x0;

	this->timelineData = 0x0;
	this->timelineWidth = 0x0;
	this->timelineHeight = 0x0;

	this->frameTimeline = 0x0;
	this->frameTimelineOriginal = 0x0;
	this->frameTimelineEdited = 0x0;
}

/************************************************************************
 * Metodo que � executado quando a thread do VideoPlayer for iniciada.
 * Pega frame a frame e emite sinal para a interface desenhar.
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Hist�rico
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/

void VideoPlayer::run()
{
	Video *vdo = currentProject->getVideo();
	Frame *frame = 0x0;

	if (!vdo)
		return;

	if ((vdo->getCurrentPosition() >= vdo->getFramesTotal()))
	{
		vdo->seekFrame(0);
	}
		
	frame = vdo->getNextFrame();

	while (frame) // Enquanto houver frame vai passando o v�deo.
	{

		// Atualiza o frame do player
		updatePlayer(frame);

		// Se o histograma estiver visivel, atualiza 
		if (mfit_ui->ui.histogramLabel->isVisible())
		{
			updateHist(frame);
		}

		delete frame;

		// Damos um tempo para fazer o v�deo passar mais devagar. Parecendo um player real.
		usleep(cvRound(vdo->getFPS()*100));
		
		// Pega o proximo frame.
		frame = vdo->getNextFrame();

	}
}

/*************************************************************************
 * M�todo que emite um sinal para o connect da interface fazer a altera��o
 * do frame a ser exibido no player
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Hist�rico
 * 09/10/08 - Thiago Mizutani
 * Revis�o - Alterado o nome da fun��o e separa��o e exclus�o de fun��es
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/

void VideoPlayer::updatePlayer(Frame *frame)
{
	// Se nao vir nada, aborta
	if (!frame)
		return;

	QImage *image = 0x0;

	image = frame->IplImageToQImage(&imageData, &imgWidth, &imgHeight);

	emit setNewFrame(image);
}

/*************************************************************************
 * M�todo que envia um sinal para o connect da interface fazer a atualiza
 * ��o do histograma.
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Hist�rico
 * 09/10/08 - Thiago Mizutani
 * Diminuindo o n�mero de fun��es e separando as coisas.
 * 29/09/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/

void VideoPlayer::updateHist(Frame *frame)
{
	QImage *imageHist = 0x0;
	Frame* frameGray = 0x0;
	Frame* frameHistogram = 0x0;
	Color* color = new Color;
	Histogram *hist = 0x0;

	frameGray = color->convert2Gray(frame);
	hist = frameGray->createHistogram();

	frameHistogram = new Frame(hist->getMatrix(), 256, hist->getMax());

	imageHist = frameHistogram->IplImageToQImage(&histData, &histWidth, &histHeight);

	// Manda exibir o histograma
	emit setHistogram(imageHist);

	delete frameGray;
	delete frameHistogram;
	delete hist;
	delete color;
}


/*
	if (ui.widgetHistogram->isVisible())
	{
// Se a janela do histograma estiver visivel: mostra o histograma
// QImage* qImage, IplImage* cvImage
Frame *newFrame = 0;
Frame *newGrayFrame = 0;
Frame *frameHistogram = 0;
QImage *imgHistogram = 0;
Histogram *hist;
Color *color = new Color;

IplImage *cvImage = cvCreateImageHeader(cvSize(image->width(), image->height ()), IPL_DEPTH_8U, 4);
cvImage->imageData = (char*)image->bits();

newFrame = new Frame(cvImage);
newGrayFrame = color->convert2Gray(newFrame);

hist = newGrayFrame->createHistogram();

frameHistogram = new Frame(hist->getMatrix(), 256, hist->getMax());

//imgHistogram = frameHistogram->IplImageToQImage();
imgHistogram = frameHistogram->IplImageToQImage(&vdo_player->imageData, &vdo_player->imgWidth, &vdo_player->imgHeight);


QPixmap pix_image = QPixmap::fromImage(*imgHistogram);

ui.histogramLabel->setScaledContents(true);
ui.histogramLabel->setPixmap(pix_image);

delete newFrame;
delete newGrayFrame;
delete hist;
delete color;
delete frameHistogram;

}
 */