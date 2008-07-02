#include "cv.h"
#include "highgui.h"

#include "../include/Histogram.h"
#include "../include/Time.h"
#include "../include/Take.h"
#include "../include/Frame.h"

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

CvCapture* open(char *vdoSrc)
{
	CvCapture *video = 0;

	video = cvCaptureFromAVI(vdoSrc);

	return (video);
}
