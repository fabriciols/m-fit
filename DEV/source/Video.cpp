#include "cv.h"
#include "highgui.h"

#include "../include/Histogram.h"
#include "../include/Time.h"
#include "../include/Take.h"
#include "../include/Frame.h"

#include "../include/Log.h"

#include "../include/Video.h"

/************************************************************************
* Fun��o que realiza o carregamento do v�deo no sistema. 
*************************************************************************
* param (E): CvCapture *vdoSrc => v�deo a ser carregado.
************************************************************************
* Retorno: Video Carregado
************************************************************************
* Hist�rico
* 01/07/08 - Thiago Mizutani
* Cria��o.
************************************************************************/
CvCapture* Video::open(char *vdoSrc)
{
	CvCapture *video = 0;

	video = cvCaptureFromAVI(vdoSrc);

	if (video == NULL)
	{
		Log::writeLog("%s :: Open Failed", __FUNCTION__);
		throw "Open failed";
	}
	
	return (video);
}

/************************************************************************
* Construtor que recebe uma string que � o nome do arquivo de video 
* a ser aberto
*************************************************************************
* param (E): char *filename_cy => Nome do arquivo a ser aberto
************************************************************************
* Retorno: Video Carregado
************************************************************************
* Hist�rico
* 01/07/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Video::Video(char *filename_cy)
{
	
	char file_cy;
	char path_cy;
	char idx_c = 0;
	char char_c;
	int i;

	// Abre o arquivo

	Log::writeLog("%s :: open filename[%s]", __FUNCTION__, filename_cy);

	this->data = open(filename_cy);

	// Atualiza as variaveis internas do objeto video

	#ifdef __linux__
		char_c = '/';
	#else
		char_c = '\\';
	#endif

	// Desmembra nome de arquivo - path

	// Procura pelo separador
	for (i = strlen(filename_cy) ; i >= 0 ; i--)
	{
		if (filename_cy[i] == char_c)
		{
			idx_c = i;
			break;
		}
	}

	Log::writeLog("%s :: open idx_c[%c]", __FUNCTION__, idx_c);
	
	if (idx_c) // Se encontrou o separador, desmembra
	{
		strncpy(this->path, filename_cy, idx_c);
		strncpy(this->name, &filename_cy[idx_c+1], strlen(filename_cy)-idx_c);
	}
	else // Sen�o, o arquivo est� no diret�rio corrente
	{
		strcpy(this->path, "./");
		strcpy(this->name, filename_cy);
	}

	Log::writeLog("%s :: path[%s] name[%s]", __FUNCTION__, this->path, this->name);

	// Pega as propriedades do video

	// FPS
	// CV_CAP_PROP_FPS - frame rate
	this->fps = cvGetCaptureProperty(this->data, CV_CAP_PROP_FPS);
	Log::writeLog("%s :: fps[%.0f] ", __FUNCTION__, this->fps);

	// Largura dos frames
	// CV_CAP_PROP_FRAME_WIDTH - width of frames in the video stream 
	this->framesWidth = cvGetCaptureProperty(this->data, CV_CAP_PROP_FRAME_WIDTH);
	Log::writeLog("%s :: famesWidth[%.0f] ", __FUNCTION__, this->framesWidth);

	// Altura dos frames 
	// CV_CAP_PROP_FRAME_HEIGHT - height of frames in the video stream
	this->framesHeight = cvGetCaptureProperty(this->data, CV_CAP_PROP_FRAME_HEIGHT);
	Log::writeLog("%s :: famesHeight[%.0f] ", __FUNCTION__, this->framesHeight);

	// Codec
	// CV_CAP_PROP_FOURCC - 4-character code of codec.
	this->codec = cvGetCaptureProperty(this->data, CV_CAP_PROP_FOURCC);
	Log::writeLog("%s :: codec[%.0f] ", __FUNCTION__, this->codec);

	// Total de Frames que o video tem
	//CV_CAP_PROP_FRAME_COUNT - number of frames in AVI file. 
	this->framesTotal = cvGetCaptureProperty(this->data, CV_CAP_PROP_FRAME_COUNT);
	Log::writeLog("%s :: framesTotal[%.0f] ", __FUNCTION__, this->framesTotal);

	// Atributos relativos a posicao
	updatePos();
	
}
 
/************************************************************************
* Atualiza os controles internos referente a posi��o atual do ponteiro
* de video
*************************************************************************
* param (E): Nenhum
************************************************************************
* return: Nenhum
************************************************************************
* Hist�rico
* 30/07/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
void Video::updatePos()
{
	long msec;

	//CV_CAP_PROP_POS_FRAMES - 0-based index of the frame to be decoded/captured next
	this->framePos = cvGetCaptureProperty(this->data, CV_CAP_PROP_POS_FRAMES);
	Log::writeLog("%s :: framePos[%.0f] ", __FUNCTION__, this->framePos);

	msec = (long)cvGetCaptureProperty(this->data, CV_CAP_PROP_POS_MSEC);
	Log::writeLog("%s :: timePos[%.0f] ", __FUNCTION__, msec);

	this->timePos.setTime(msec);

}

/************************************************************************
* Get para a vari�vel name
*************************************************************************
* param (E): Nenhum
************************************************************************
* return: char* name
************************************************************************
* Hist�rico
* 30/07/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
char* Video::getName()
{
	return name;
}

/************************************************************************
* Get para a vari�vel path
*************************************************************************
* param (E): Nenhum
************************************************************************
* return: char* path
************************************************************************
* Hist�rico
* 30/07/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
char* Video::getPath()
{
	return path;
}

/************************************************************************
* Get para a vari�vel framesTotal
*************************************************************************
* param (E): Nenhum
************************************************************************
* return: double framesTotal
************************************************************************
* Hist�rico
* 30/07/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
double Video::getFramesTotal()
{
	return framesTotal;
}

/************************************************************************
* Captura o frame corrente e aponta para o pr�ximo
*************************************************************************
* param (E): Nenhum
************************************************************************
* return: Frame* - frame capturado.
************************************************************************
* Hist�rico
* 30/07/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Frame* Video::getNextFrame()
{
	if (!cvGrabFrame(this->data))
	{
		return NULL;
	}
	else
	{
		Frame *frameNew = new Frame(cvRetrieveFrame(this->data));

		updatePos();

		return (frameNew);
	}
}

/************************************************************************
* Captura o frame anterior e passa a apontar para ele
*************************************************************************
* param (E): Nenhum
************************************************************************
* return: Frame* - frame capturado.
************************************************************************
* Hist�rico
* 30/07/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Frame* Video::getPreviousFrame()
{
	// Posiciono no frame anterior
	seekFrame((long)this->getCurrentPosition() - 1);

	// Capturo este frame
	Frame *frameNew = getNextFrame();

	// Posiciono no frame capturado
	seekFrame((long)this->getCurrentPosition() - 1);

	return (frameNew);
}

/************************************************************************
* Captura o frame corrente e n�o move o ponteiro para o pr�ximo
*************************************************************************
* param (E): Nenhum
************************************************************************
* return: Frame* - frame capturado.
************************************************************************
* Hist�rico
* 30/07/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Frame* Video::getCurrentFrame()
{
	// Pega o frame atual
	Frame *frameCurrent = getNextFrame();

	// Volta a apontar pra ele
	if (seekFrame((long)this->getCurrentPosition() - 1) == 1)
	{
		// Se retornar 1: erro FATAL!
		throw;
	}

	return frameCurrent;

}

/************************************************************************
* Get para a variavel framesWidth
*************************************************************************
* param (E): Nenhum
************************************************************************
* return: double framesWidth
************************************************************************
* Hist�rico
* 30/07/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
double Video::getFramesWidth()
{
	return framesWidth;
}

/************************************************************************
* Get para a variavel framesHeight
*************************************************************************
* param (E): Nenhum
************************************************************************
* return: double framesHeight
************************************************************************
* Hist�rico
* 30/07/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
double Video::getFramesHeight()
{
	return framesHeight;
}

/************************************************************************
* Posiciona o ponteiro do video.
*************************************************************************
* param (E): unsigned long posFrame - Numero do frame em quest�o
************************************************************************
* return: 	1 - Erro: parametro passado � invalido
*				0 - Sucesso
************************************************************************
* Hist�rico
* 30/07/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
int Video::seekFrame(unsigned long posFrame)
{
	if (posFrame >= this->framesTotal)
		return 1;

	cvSetCaptureProperty(this->data, CV_CAP_PROP_POS_FRAMES, (double)posFrame);

	updatePos();

	return 0;
}

/************************************************************************
* Retorna valor da posi��o atual do video (em qual frame est�)
*************************************************************************
* param (E): Nenhum
************************************************************************
* return: double framePos
************************************************************************
* Hist�rico
* 13/08/08 - Thiago Mizutani
* Renomeando a fun��o de acordo com aquilo que estava na especifica��o.
* 30/07/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
double Video::getCurrentPosition()
{
	return framePos;
}

/************************************************************************
* Retorna o valor do FPS do v�deo
*************************************************************************
* param (E): Nenhum
************************************************************************
* return: double FPS 
************************************************************************
* Hist�rico
* 15/08/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

double Video::getFPS()
{
	return (this->fps);
}