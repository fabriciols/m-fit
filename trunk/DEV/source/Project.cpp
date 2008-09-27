#include "cv.h"
#include <QImage>
#include "highgui.h"
#include <vector>

#include "../include/Interface.h"

#include "../include/VisualRythim.h"
#include "../include/Transition.h"
#include "../include/Project.h"
#include "../include/Log.h"

#include "../include/main.h"

int Project::openProject(char *filename_cy)
{
	// Abre um projeto ja existente

	// Troca o titulo da janela
	mfit_ui->changeWindowTitle(this->filename_cy);

	return true;
}

Project::Project(void)
{
	// Inicializa os ponteiros com 0
	strcpy(this->filename_cy, "NewProject.mfit");

	// Troca o titulo da janela
	mfit_ui->changeWindowTitle(this->filename_cy);

}

int Project::openVideo(QString fileName)
{
	/*
		if (this->vdo != NULL)
		return false;
		*/
	char param_cy[50];
	char *filename_cy;

	QByteArray ba = fileName.toLatin1();
	filename_cy = ba.data(); 

	this->vdo = new Video(filename_cy);

	// Pega as propriedades do video e exibe na lista
	//
	//
	memset(param_cy, '\0', sizeof(param_cy));
	
	// Nome
	mfit_ui->insertVideoProperty("Name", this->vdo->getName());

	// Path
	mfit_ui->insertVideoProperty("Path", this->vdo->getPath());

	// Codec
	memset(param_cy, '\0', sizeof(param_cy));
	sprintf(param_cy, "%.0lf", this->vdo->getCodec());
	mfit_ui->insertVideoProperty("Codec", param_cy);

	// framesHeight
	memset(param_cy, '\0', sizeof(param_cy));
	sprintf(param_cy, "%.0lf", this->vdo->getFramesHeight());
	mfit_ui->insertVideoProperty("Height", param_cy); 

	// framesWidth
	memset(param_cy, '\0', sizeof(param_cy));
	sprintf(param_cy, "%.0lf", this->vdo->getFramesHeight());
	mfit_ui->insertVideoProperty("Width",  param_cy);

	// FPS
	memset(param_cy, '\0', sizeof(param_cy));
	sprintf(param_cy, "%.2lf", this->vdo->getFPS());
	mfit_ui->insertVideoProperty("FPS",  param_cy);

	// Frames
	memset(param_cy, '\0', sizeof(param_cy));
	sprintf(param_cy, "%.0lf", this->vdo->getFramesTotal());
	mfit_ui->insertVideoProperty("Frames",  param_cy);

	mfit_ui->updateVideoPlayer(this->vdo->getCurrentFrame());

	return true;
}

Video* Project::getVideo()
{
	return vdo;
}
