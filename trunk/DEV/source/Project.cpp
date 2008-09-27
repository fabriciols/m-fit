#include "cv.h"
#include <QImage>
#include "highgui.h"
#include <vector>


#include "../include/Time.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"
#include "../include/VisualRythim.h"
#include "../include/Transition.h"
#include "../include/Interface.h"
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
	this->vdo         = 0;
	this->frame       = 0;

	memset(this->path_cy, '\0', sizeof(this->path_cy));

	// Inicializa o nome de projeto padrao
	strcpy(this->filename_cy, "NewProject.mfit");

	// Troca o titulo da janela
	mfit_ui->changeWindowTitle(this->filename_cy);

}
