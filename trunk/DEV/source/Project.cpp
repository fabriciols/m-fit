#include "cv.h"
#include <QImage>
#include "highgui.h"
#include <vector>

#include "../include/Interface.h"

#include "../include/VisualRythim.h"
#include "../include/Transition.h"
#include "../include/Project.h"
#include "../include/Xml.h"
#include "../include/Log.h"

#include "../include/main.h"

/************************************************************************
* Abre um arquivo de projeto, que eh um XML, parsea ele e sobe as
* estruturas.
* TODO: ParserXML
*************************************************************************
* param (E): Nenhum
*************************************************************************
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
int Project::openProject(QString fileName)
{
	char filename_cy[100];
	char content[100], attr1[50], attr2[50], attr3[50];
	int sizeTag=0;
	int ret,i;
//	char txt[100];
	Xml *fileXml = new Xml();
	// Abre um projeto ja existente

	mfit::QStringToChar(fileName, filename_cy);

	ret = fileXml->openXml(filename_cy);

	if(!ret)
	{
		fileXml->readXml("name", content, attr1, attr2, attr3, &sizeTag, 0);

		mfit_ui->changeWindowTitle(content);

		fileXml->readXml("video", content, attr1, attr2, attr3, &sizeTag, 0);

		openVideo(content);

		for(i=0;i<sizeTag;i++)
		{
			fileXml->readXml("transition", content, attr1, attr2, attr3, &sizeTag, i);
			Transition *transition = new Transition(atoi(attr1), atol(attr2), content);
			this->transitionList.push_back(*transition);
		}
	}

	fileXml->closeXml();
	
	return true;
}

/************************************************************************
* Inicia um novo projeto
*************************************************************************
* param (E): Nenhum
*************************************************************************
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
Project::Project(void)
{
	// Inicializa os ponteiros com 0
	this->vdo = '\0';
	this->frame = '\0';
	this->userCutThreshold = '\0';

	strcpy(this->filename_cy, "NewProject.mfit");

	// Troca o titulo da janela
	mfit_ui->changeWindowTitle(this->filename_cy);

}

/************************************************************************
* Realiza a abertura de um video novo.
* Preenche a videoPropertiesTree com informacoes do video.
* Caso ja tenha um video aberto, mata ele e limpa a videoPropertiesTree.
*************************************************************************
* param (E): Nenhum
*************************************************************************
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
int Project::openVideo(QString fileName)
{
	char param_cy[50];
	char *filename_cy;

	Log::writeLog("%s :: vdo[%x]", __FUNCTION__, this->vdo);

	if (this->vdo != 0x0)
	{
		Log::writeLog("%s :: cleaning vdo[%x]", __FUNCTION__, this->vdo);
		delete this->vdo;
		mfit_ui->clearVideoProperty();
		mfit_ui->clearTransitionsTree();
	}

	// Transformando QString em char*
	QByteArray ba = fileName.toLatin1();
	filename_cy = ba.data(); 

	this->vdo = new Video(filename_cy);

	// Pega as propriedades do video e exibe na lista
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

	mfit_ui->createTimeline();

	return true;
}

/************************************************************************
* Retorna o video corrente do projeto
*************************************************************************
* param (E): Nenhum
*************************************************************************
* return : Video* -> video do projeto
*************************************************************************
* Histórico
* 29/09/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/

Video* Project::getVideo()
{
	return vdo;
}

/************************************************************************
* Define o limiar do projeto com o valor de entrada do usuário.
*************************************************************************
* param (E): int threshold -> valor do limiar. 
*************************************************************************
* return : Nenhum. 
*************************************************************************
* Histórico
* 14/10/08 - Thiago Mizutani
* Criação.
************************************************************************/

void Project::setUserThreshold(int threshold)
{
	this->userCutThreshold = threshold;
}

/************************************************************************
* Retorna o valor do limiar definido pelo usuário no projeto.
*************************************************************************
* param (E): Nenhum
*************************************************************************
* return : int userCutThreshold -> limiar definido pelo usuário. 
*************************************************************************
* Histórico
* 14/10/08 - Thiago Mizutani
* Criação.
************************************************************************/

int Project::getUserThreshold()
{
	return (this->userCutThreshold);
}
