#include "cv.h"
#include "highgui.h"


#include <QImage>
#include <vector>

#include "../include/Interface.h"
#include "../include/Effect.h"
#include "../include/Color.h"
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
	char content[100], type[50], posTransition[50], posUserTransition[50], userCutThreshold[50];
	int sizeTag=0;
	int ret,i;
	Xml *fileXml = new Xml();
	// Abre um projeto ja existente

	mfit::QStringToChar(fileName, filename_cy);

	ret = fileXml->openXml(filename_cy);

	if(!ret)
	{
		//apaga lista de transicoes
		this->transitionList.clear();

		fileXml->readXml("name", content, type, posTransition, posUserTransition, userCutThreshold, &sizeTag, 0);

		mfit_ui->changeWindowTitle(content);

		fileXml->readXml("video", content, type, posTransition, posUserTransition, userCutThreshold, &sizeTag, 0);

		openVideo(content);

		for(i=0;i<sizeTag;i++)
		{
			fileXml->readXml("transition", content, type, posTransition, posUserTransition, userCutThreshold, &sizeTag, i);

			if(sizeTag)
			{
				Transition *transition = new Transition(atoi(type), atol(posTransition), content);
				this->transitionList.push_back(*transition);
			}
		}
	}

	fileXml->closeXml();
	
	return true;
}

/************************************************************************
 * Salva um arquivo de projeto, que eh um XML, parsea ele e sobe as
 * estruturas.
 * TODO: ParserXML
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Histórico
 * 14/10/08 - Ivan Shiguenori Machida
 * Criação.
 ************************************************************************/
int Project::saveProject(QString fileName)
{
	char *filename_cy;
	char txt[255];
	int ret;
	Xml *fileXml = new Xml();

	QByteArray ba = fileName.toLatin1();
	filename_cy = ba.data(); 

	sprintf(txt, "%s/%s", this->vdo->getPath(), this->vdo->getName());

	ret = fileXml->createXml(filename_cy, NULL, txt, &this->transitionList);

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
	this->userMinCanny = '\0';
	this->userMaxCanny = '\0';

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
		
		//apaga lista de transicoes
		this->transitionList.clear();

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

/*************************************************************************
 * Função set para limiar mínimo (definido pelo usuario) do filtro de canny 
 *************************************************************************
 * param (E) : int userMinCanny-> limiar minimo definido pelo usuário. 
 *************************************************************************
 * return : nenhum
 *************************************************************************
 * Histórico
 * 18/10/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

void Project::setUserMinCanny(int userMinCanny)
{
	this->userMinCanny = userMinCanny;
}

/*************************************************************************
 * Função get para limiar mínimo (definido pelo usuario) do filtro de canny 
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * return : int userMinCanny -> limiar minimo definido pelo usuário. 
 *************************************************************************
 * Histórico
 * 18/10/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

int Project::getUserMinCanny()
{
	return (this->userMinCanny);
}

/*************************************************************************
 * Função set para limiar máximo (definido pelo usuario) do filtro de canny 
 *************************************************************************
 * param (E):int userMaxCanny -> limiar maximo definido pelo usuário 
 *************************************************************************
 * return : não há. 
 *************************************************************************
 * Histórico
 * 18/10/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

void Project::setUserMaxCanny(int userMaxCanny)
{
	this->userMaxCanny = userMaxCanny;
}

/*************************************************************************
 * Função get para limiar máximo (definido pelo usuario) do filtro de canny 
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * return : int userMaxCanny-> limiar máximo definido pelo usuário. 
 *************************************************************************
 * Histórico
 * 18/10/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

int Project::getUserMaxCanny()
{
	return (this->userMaxCanny);
}

/*************************************************************************
 * Ordena a lista de transiçõs
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * return : Nenhum. 
 *************************************************************************
 * Histórico
 * 17/10/08 - Fabrício Lopes de Souza 
 * Criação.
 ************************************************************************/

void Project::sortTransitionList(void)
{
	sort(this->transitionList.begin(), this->transitionList.end() );
}

long Project::FrameToTimelinePos(long frame)
{
	// A formula para saber em que ponto plotar o indicador é:
	// SIZE_FRAME_TIMELINE  ---- SIZE_SEC_FRAME*vdo->getFPS()
	// pos (ponto timeline) ---- pos (ponto no video)
	return (SIZE_FRAME_TIMELINE*cvRound(frame)) / (SIZE_SEC_FRAME*cvRound(this->vdo->getFPS()));
}

long Project::TimelinePosToFrame(long pos)
{
	// A ideia aqui e a seguinte:
	// Tendo em maos as seguintes variaveis:
	// Todo FRAME da timeline tem o tamanho fixo de SIZE_FRAME_TIMELINE
	// E cada um desse FRAME compreende SIZE_SEC_FRAME segundos do video
	// captando a posicao em que o kr clicou, podemos fazer a seguinte regra de 3:
	// SIZE_FRAME_TIMELINE ---------- SIZE_SEC_FRAME*vdo->getFPS()
	// Posicao clicada (x) ---------- X (posicao no frame)
	return (cvRound(SIZE_SEC_FRAME*this->vdo->getFPS())*pos / SIZE_FRAME_TIMELINE);
}

/*************************************************************************
 * Salva vídeo após aplicação de efeitos.
 *************************************************************************
 * param (E): char *filename_cy 
 *************************************************************************
 * return : nenhum.
 *************************************************************************
 * Histórico
 * 16/10/08 - Fabrício Lopes de Souza
 * Criação.
 ************************************************************************/

void Project::renderVideo(char *filename_cy)
{
	CvVideoWriter* videoWriter;
	long totalFrame = 0x0;
	long i = 0;
	long currentPos = 0;
	unsigned int j = 0;
	unsigned int numEffect = 0;
	int ret_i = 0;

	// Teste
	{
		Effect *color  = new Color(0,100);
		effectList.push_back(color);
	}

	Frame *frame;
	Frame *frameEffect;
	Effect *effect;

	// CODECS
	// CV_FOURCC('H','F','Y','U') // HuffYUV  ~3:1 compression.
	// CV_FOURCC('P','I','M','1') // MPEG 1 (if you have it)
	// CV_FOURCC('M','J','P','G') // Motion-jpeg
	// CV_FOURCC('D','I','B',' ') // Uncompressed
	// CV_FOURCC('C','D','V','C') // Canopus DV
	// CV_FOURCC('D','I','V','X') // divx

	// Abre o writer do video
	videoWriter = cvCreateAVIWriter(filename_cy, CV_FOURCC('D','I','B',' '),
			vdo->getFPS(), cvSize(cvRound(vdo->getFramesHeight()), cvRound(vdo->getFramesWidth())));

	// Posiciona o ponteiro no comeco do video
	currentPos = (long)vdo->getCurrentPosition();
	vdo->seekFrame(0);

	totalFrame = (long)vdo->getFramesTotal();

	// Carregamos o primeiro efeito, caso tiver
	if (effectList.size() > 0)
	{
		numEffect = effectList.size();
		effect = effectList.at(0);
	}
	else
	{
		effect = 0x0;
	}

	// Varremos Frame a Frame do video
	for ( i = 0 , j = 0 ; i < totalFrame ; i ++)
	{
		frame = vdo->getNextFrame();

		// Checamos se temos efeito a aplicar no frame indicado
		if (effect)
		{
			// Se o frame corrente for maior que o primeiro frame do efeito
			if (i >= effect->getFrameStart())
			{
				if (i <= effect->getFrameEnd())
				{
					// Aplica o efeito no frame
					frameEffect = effect->applyEffect(frame);
					delete frame;
					frame = frameEffect;
				}
				else
				{ // Acabou este efeito, vamos para o proximo

					j++;
					// Se ainda restarem efeitos
					if (j <= (numEffect-1))
					{
						effect = effectList.at(j);
					}
					else
					{
						effect = 0x0;
					}
				}
			}
		}

		// Escreve o frame no video final
		ret_i = cvWriteToAVI(videoWriter, frame->data);

		// Apaga o ponteiro para o video
		delete frame;
	}

	// Libera o novo video
	cvReleaseVideoWriter(&videoWriter);

	// Devolve o ponteiro pra posicao inicial
	vdo->seekFrame(currentPos);

}
