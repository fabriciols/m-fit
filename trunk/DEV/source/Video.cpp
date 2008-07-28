#include "cv.h"
#include "highgui.h"

#include "../include/Histogram.h"
#include "../include/Time.h"
#include "../include/Take.h"
#include "../include/Frame.h"

#include "../include/Log.h"

#include "../include/Video.h"

/************************************************************************
* Função que realiza o carregamento do vídeo no sistema. 
*************************************************************************
* param (E): CvCapture *vdoSrc => vídeo a ser carregado.
************************************************************************
* Retorno: Video Carregado
************************************************************************
* Histórico
* 01/07/08 - Thiago Mizutani
* Criação.
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
* Construtor que recebe uma string que é o nome do arquivo de video 
* a ser aberto
*************************************************************************
* param (E): char *filename_cy => Nome do arquivo a ser aberto
************************************************************************
* Retorno: Video Carregado
************************************************************************
* Histórico
* 01/07/08 - Fabricio Lopes de Souza
* Criação.
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
	else // Senão, o arquivo está no diretório corrente
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
	// na instancia do objeto, estao todos em 0
	this->framePos = 0;
	
}

char* Video::getName()
{
	return name;
}

char* Video::getPath()
{
	return path;
}

double Video::getFramesTotal()
{
	return framesTotal;
}

Frame* Video::getNextFrame()
{
	if (!cvGrabFrame(this->data))
	{
		return NULL;
	}
	else
	{
		return (new Frame(cvRetrieveFrame(this->data)));
	}
}

Frame* Video::getCurrentFrame()
{
	return getNextFrame();
}

double Video::getFramesWidth()
{
	return framesWidth;
}

double Video::getFramesHeight()
{
	return framesHeight;
}
