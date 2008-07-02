#include "cv.h"
#include "highgui.h"

#include "../include/Histogram.h"
#include "../include/Time.h"
#include "../include/Take.h"
#include "../include/Frame.h"

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

CvCapture* open(char *vdoSrc)
{
	CvCapture *video = 0;

	video = cvCaptureFromAVI(vdoSrc);

	return (video);
}
