#include "cv.h"
#include "highgui.h"

#include <stdexcept>
#include <math.h>

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Log.h"

#define HIST_WIDTH  256
#define HIST_HEIGHT 256

/************************************************************************
 * Auxiliar que atualiza as variaveis internas de um objeto Frame 
 * deve ser usada toda vez que for alterar a imagem de um frame.
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Histórico:
 * 29/07/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
void Frame::setImage(IplImage* imgNew)
{
	Log::writeLog("%s :: old [%x] new [%x]", __FUNCTION__, this->data, imgNew);

	// Se ja tiver algum alocado
	if (this->data)
	{
		// Desaloca
		imgDealloc(this->data);
	}

	// Aponta para a img nova
	this->data = imgNew;

	// Aponta para os valores do IplImage
	this->width =  &this->data->width;
	this->height = &this->data->height;

}

/************************************************************************
 * Inicializa todos os atributos com nulo
 * Esta função deve ser usada em TODOS os construtores
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Histórico:
 * 29/07/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
void Frame::initAttr()
{
	this->data = NULL;
	this->width =  NULL;
	this->height = NULL;
}

/************************************************************************
 * Construtor que somente inicializa as variavies de controle com nulo 
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Histórico:
 * 29/07/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
Frame::Frame()
{
	initAttr();
}

/************************************************************************
 * Construtor para Frame que recebe os valores necessários para plotar
 * um histograma. Cria um objeto Frame com o desenho do histograma.
 *************************************************************************
 * param (E): double matrix[] - Valores do Histograma
 * param (E): int len_i       - Quantidade de valores em 'matrix'
 * param (E): float max_f     - Valor máximo do histograma
 *************************************************************************
 * Histórico:
 * 27/06/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
Frame::Frame(double *matrix, int len_i, float max_f)
{

	int i;
	int normalized;
	char *frame_cy;
	CvFont font;
	CvPoint point_new  = {0,0};
	CvPoint point_last = {0,0};

	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 0.5, 0.5, 0, 1, CV_AA);

	frame_cy = (char*)malloc(sizeof(char)*10);

	IplImage* imgHistogram = 0;

	//imgHistogram = cvCreateImage(cvSize(len_i*2, HIST_HEIGHT), 8, 1);
	imgHistogram = imgAlloc(cvSize(len_i, HIST_HEIGHT), 8, 1);

	// Desenha a area de trabalho do histograma
	//cvRectangle(imgHistogram, cvPoint(0,0), cvPoint(len_i*2,HIST_HEIGHT), CV_RGB(255,255,255), -1);
	cvRectangle(imgHistogram, cvPoint(0,0), cvPoint(len_i,HIST_HEIGHT), CV_RGB(255,255,255), -1);

	// Desenhar as linhas do histograma
	for (i=0 ; i < len_i ; i++)
	{
		normalized = cvRound((matrix[i]*HIST_HEIGHT)/max_f);

		//Log::writeLog("%s :: Plot Value[%3d] = [%4.0lf] Normalized = [%3d]", __FUNCTION__, i, matrix[i], normalized);

		// Printa a linha do Histograma
		//cvLine(imgHistogram, cvPoint((i*2)-1,HIST_HEIGHT), cvPoint((i*2)-1,HIST_HEIGHT-normalized), CV_RGB(0, 0, 0));
		//cvLine(imgHistogram, cvPoint((i)-1,HIST_HEIGHT), cvPoint((i)-1,HIST_HEIGHT-normalized), CV_RGB(0, 0, 0));
		point_new = cvPoint((i-1), HIST_HEIGHT-normalized);

		//cvCircle(imgHistogram, point_new, 0.1, cvScalar(0,0,0), 1);
		cvLine(imgHistogram, point_new , point_new, cvScalar(0,0,0), 1);
		cvLine(imgHistogram, point_last, point_new, cvScalar(0,0,0), 1);

		point_last = point_new;

		if (i % 32 == 0)
		{
			sprintf(frame_cy, "%d", i);
			//Log::writeLog("%s :: i[%d] == cvPutText :[%s] cvPoint(%d,%d)", __FUNCTION__, i, frame_cy, i-1, HIST_HEIGHT);
			cvPutText(imgHistogram, frame_cy, cvPoint(i-1,10), &font, CV_RGB(0,0,0));
			//cvLine(imgHistogram, cvPoint((i)-1,0), cvPoint((i)-1,HIST_HEIGHT), CV_RGB(0, 0, 0));
		}


		// Print uma linha em branco, pro histograma nao ficar todo grudado
		//cvLine(imgHistogram, cvPoint(i*2,HIST_HEIGHT), cvPoint(i*2,HIST_HEIGHT-normalized), CV_RGB(0, 255, 255));

	}

	initAttr();
	setImage(imgHistogram);

}

/************************************************************************
 * Construtor para Frame que recebe um caminho para um arquivo no disco.
 * Tenta abrir o arquivo de imagem e constroi um objeto Frame a partir dele
 *************************************************************************
 * param (E): char* filename_cy - String com o caminho para a imagem.
 *************************************************************************
 * Histórico:
 * 27/06/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
Frame::Frame(char *filename_cy)
{
	IplImage* img;

	img = cvLoadImage(filename_cy, 1);

	if (img == NULL)
	{
		throw "Cant open file";
	}

	initAttr();
	setImage(img);
}

/************************************************************************
 * Construtor para Frame que recebe uma imagem já carregada
 * faz uma copia dessa imagem, e deixa o endereco no frame.
 * Cria um Frame com base nos dados da imagem passada.
 *************************************************************************
 * param (E): IplImage* img_src - Imagem a ser usada como base para a
 * criação de um Frame
 *************************************************************************
 * Histórico:
 * 27/06/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
Frame::Frame(IplImage *img_src)
{
	IplImage *imgNew;

	Log::writeLog("%s :: Constructor param: IplImage[%x]", __FUNCTION__, img_src);
	initAttr();

	imgNew = imgAlloc(cvSize(img_src->width, img_src->height), img_src->depth, img_src->nChannels);

	imgCopy(img_src, imgNew);

	setImage(imgNew);
}
/************************************************************************
 * Construtor para Frame que recebe um frame já instanciado.
 * Clona o frame.
 *************************************************************************
 * param (E): Frame *frame - Frame a ser clonado.
 *************************************************************************
 * Histórico:
 * 27/06/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
Frame::Frame(Frame *frame)
{
	IplImage *img;

	// Faz a copia do objeto
	img = imgAlloc(cvSize(frame->getWidth(), frame->getHeight()), frame->data->depth, frame->data->nChannels);

	imgCopy(frame->data, img);

	initAttr();
	setImage(img);

}

/************************************************************************
 * Função que retorna a largura do frame.
 *************************************************************************
 * param (E): Nenhum.
 *************************************************************************
 * return : int - Largura do frame.
 *************************************************************************
 * Histórico:
 * 27/06/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
int Frame::getWidth()
{
	return(*this->width);
}

/************************************************************************
 * Função que retorna a altura do frame.
 *************************************************************************
 * param (E): Nenhum.
 *************************************************************************
 * return : int - Altura do frame. 
 *************************************************************************
 * Histórico:
 * 27/06/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
int Frame::getHeight()
{
	return(*this->height);
}

/************************************************************************
 * FUNÇÃO QUE PEGA A DIAGONAL DE UM FRAME. 
 *************************************************************************
 * Esta função utiliza as fórmulas de trigonometria para encontrar a equa
 * ção da reta a partir disto obter todos os pontos da diagonal do frame. 
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * return : Frame* -> Frame com a diagonal
 *************************************************************************
 * Histórico:
 * 29/07/08 - Fabrício Lopes de Souza
 * 'N' alterações para fazer funcionar, precisa-se validar com perfeição
 * para verificar se está correto.
 * 15/07/08 - Thiago Mizutani
 * Adequação do processo de montagem da diagonal do frame. Utilizando as
 * funções de getPixel e set Pixel.
 * 06/07/08 - Thiago Mizutani
 * Ajustes para extracao dos pontos da diagonal principal do frame.
 * 04/07/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/
Frame * Frame::getDiagonal()
{
	int x = 0; // x da equacao da reta
	int y = 0; // y da equacao da reta

	Frame* frameDiagonal;

	int luminance = 0; //Valor de luminancia do pixel retirado da diagonal.

	double a = 0; // Coeficiente angular da equacao

	Log::writeLog("%s :: diagonal img width[%d] height[%d]", __FUNCTION__, 1, this->getWidth());

	IplImage* imgDiagonal = imgAlloc(cvSize(1, this->getWidth()-1), 8, 1);

	frameDiagonal = new Frame(imgDiagonal);


	/** Calculo o coeficiente angular da reta ('a' da equacao).
	 * this->getHeight = y - yo
	 * this->getWidth  = x - xo
	 * y - yo = m*(x - xo)
	 **/
	a = (float)(this->getHeight()/(float)this->getWidth());

	// Pego a diagonal do this.
	for (x=0; x < this->getWidth()-1 ; x++)
	{
		y = cvRound(a * x);

		// Monto o this da diagonal (já transformado em coluna).
		frameDiagonal->setPixel(0, x, this->getPixel(x, y));
	}	

	imgDealloc(imgDiagonal);

	return(frameDiagonal);
}

/************************************************************************
 * Função que define a luminancia de um pixel
 *************************************************************************
 * param (E): int x   -> coordenada x do pixel desejado
 * param (E): int y   -> coordenada y do pixel desejado
 * param (E): int lum -> valor de luminancia a ser definido no pixel (x,y)
 *************************************************************************
 * Histórico:
 * 14/07/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
void Frame::setPixel(int x, int y, int lum)
{
	CvScalar s;

	s.val[0] = lum;

	cvSet2D(this->data, y, x, s);
}

/************************************************************************
 * Função que retorna o valor de luminancia de um determinado pixel (x,y)
 *************************************************************************
 * param (E): int x   -> coordenada x do pixel desejado
 * param (E): int y   -> coordenada y do pixel desejado
 *************************************************************************
 * Histórico:
 * 14/07/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
int Frame::getPixel(int x, int y)
{
	//return ((uchar*)(this->data->imageData + this->data->widthStep*y))[x];
	CvScalar s;

	s = cvGet2D(this->data, y, x);

	return cvRound(s.val[0]);
}

/************************************************************************
 * Destructor da classe frame. Libera a memoria da imagem
 *************************************************************************
 * param (E): Nenhum
 *************************************************************************
 * Histórico:
 * 28/07/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
Frame::~Frame()
{
	// Se o ponteiro para a imagem nao for nulo
	if (this->data)
	{
		Log::writeLog("%s :: Deleting [%x]", __FUNCTION__, this->data);
		// Libera a memoria alocada para ele
		imgDealloc(this->data);
	}
}

/************************************************************************
 * Sobrecarga do operador '+=' ele ira realizar a concatenacao dos frames
 * Obs.: Atencao para os parametros, pois o soma tem que ser a dois objetos 
 * absolutos, e nao dois ponteiros
 *************************************************************************
 * param (E): Frame frame -> Frame a ser concatenado
 *************************************************************************
 * Histórico:
 * 28/07/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
Frame & Frame::operator+=(Frame &frame)
{

	IplImage *imgDst;

	Log::writeLog("%s :: this->data[%x]", __FUNCTION__, this->data);

	// As imagens tem que ter obrigatoriamente a mesma altura
	if (this->getHeight() != frame.getHeight())
		throw;

	// Crio uma imagem com a largura igual a soma das larguras
	imgDst = imgAlloc(cvSize(this->getWidth() + frame.getWidth(), this->getHeight()), this->data->depth, this->data->nChannels);

	Log::writeLog("%s :: img_dst width[%d] height[%d]", __FUNCTION__, imgDst->width, imgDst->height);

	Log::writeLog("%s :: this x[%d] y[%d] width[%d] height[%d]", __FUNCTION__, 0, 0, this->getWidth(), this->getHeight());

	// Na imagem destino, seto a area de interesse o espaco que a primeira imagem ira ocupar
	cvSetImageROI(imgDst,
			cvRect
			(
			 0,
			 0,
			 this->getWidth(),
			 this->getHeight())
			);

	// então copio esta imagem para esta área
	imgCopy(this->data,imgDst);

	Log::writeLog("%s :: frame x[%d] y[%d] width[%d] height[%d]", __FUNCTION__, this->getWidth(), 0, frame.getWidth(), frame.getHeight());

	// Agora, seto a area de interesse o espaco que a segunda imagem irá ocupar
	cvSetImageROI(imgDst,
			cvRect
			(
			 this->getWidth(),
			 0,
			 frame.getWidth(),
			 frame.getHeight())
			);

	// Copia a segunda parte do frame
	imgCopy(frame.data,imgDst);

	// Removo as áreas de interesse da imagem
	cvResetImageROI(imgDst);

	// Seta pra imagem nova
	setImage(imgDst);

	Log::writeLog("%s :: this->data[%x]", __FUNCTION__, this->data);

}

/************************************************************************
 * Função que chama o construtor do histograma e retorna o histograma
 * do frame. 
 *************************************************************************
 * param (E): IplImage* frame => Frame do qual será gerado o histograma
 * param (S): Histogram* &histogram => Histograma do frame de entrada
 *************************************************************************
 * Histórico:
 * 29/07/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

Histogram* Frame::createHistogram()
{
	return new Histogram(this->data);
}

/************************************************************************
 * Escreve a imagem em arquivo
 *************************************************************************
 * param (E): char *filename_cy -> nome do arquivo ser gravado
 *************************************************************************
 * Histórico:
 * 05/08/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
void Frame::write(char *filename_cy)
{
	Log::writeLog("%s :: write file = [%s]", __FUNCTION__, filename_cy);

	if (this->data)
	{
		if(!cvSaveImage(filename_cy, this->data))
		{
			printf("Could not save: %s\n",filename_cy);
		}
	}
	else
	{
		Log::writeLog("%s :: No image to save", __FUNCTION__);
	}
}

/************************************************************************
 * Sobrecarga do operador '='
 *************************************************************************
 * param (E): Frame frame -> Frame a ser copiado
 *************************************************************************
 * Histórico:
 * 05/08/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
Frame & Frame::operator=(Frame &frame)
{
	IplImage *imgNew;

	if (this->data)
	{
		Log::writeLog("%s :: Release: this->data[%x]", __FUNCTION__, this->data);
		imgDealloc(this->data);
	}

	imgNew = imgAlloc(cvSize(frame.getWidth(), frame.getHeight()), frame.data->depth, frame.data->nChannels);

	imgCopy(frame.data, imgNew);

	initAttr();

	setImage(imgNew);

	return *this;

}
/************************************************************************
 * Funcao que realiza a copia de uma imagem
 *************************************************************************
 * param (E): IplImage* imgSrc -> Imagem de origem
 * param (S): IplImage* imgDst -> Imagem de saida 
 *************************************************************************
 * Histórico:
 * 12/08/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
void Frame::imgCopy(IplImage *imgSrc, IplImage *imgDst)
{
	Log::writeLog("%s :: Copy : [%x] to [%x]", __FUNCTION__, imgSrc, imgDst);

	cvCopy(imgSrc, imgDst);

	imgDst->origin = imgSrc->origin;
}

/************************************************************************
 * Funcao que centraliza a alocacao de memoria e criacao de uma imagem
 *************************************************************************
 * param (E): Frame* frame -> Frame o qual sera copiado as propriedades
 *************************************************************************
 * Histórico:
 * 25/08/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
IplImage* Frame::imgAlloc(Frame* frame)
{
	IplImage *imgNew;

	imgNew = imgAlloc(cvGetSize(frame->data), frame->data->depth, frame->data->nChannels);

	return imgNew;
}
/************************************************************************
 * Funcao que centraliza a alocacao de memoria e criacao de uma imagem
 *************************************************************************
 * param (E): CvSize  size     -> Altura e largura da imagem
 * param (E): int		depth    -> Quantidade de bits para as cores
 * param (E): int		channels -> Quantidades de canais possiveis
 *************************************************************************
 * Histórico:
 * 12/08/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
IplImage* Frame::imgAlloc(CvSize size, int depth, int channels)
{
	IplImage *imgNew;

	imgNew = cvCreateImage(size, depth, channels);

	Log::writeLog("%s :: New image: [%x]", __FUNCTION__, imgNew);

	return imgNew;
}

/************************************************************************
 * Funcao que centraliza a desaalocacao de memoria de imagens
 *************************************************************************
 * param (E): IplImage* img -> Imagem a ser desalocada
 *************************************************************************
 * Histórico:
 * 12/08/08 - Fabricio Lopes de Souza
 * Criação.
 ************************************************************************/
void Frame::imgDealloc(IplImage *img)
{
	Log::writeLog("%s :: Delete image: [%x]", __FUNCTION__, img);
	cvReleaseImage(&img);
}

/************************************************************************
 * Funcao que realiza a binarização de uma imagem. Tudo aquilo que
 * possuir luminancia menor do que o limiar passado, terá seu valor 
 * alterado para 0 e tudo aquilo que tiver luminancia acima do valor do 
 * limiar terá seu valor alterado para 255.
 *************************************************************************
 * param (E): Frame* frame => imagem a ser binarizada.
 * param (E): Int threshold => Limiar para binarização
 *************************************************************************
 * Histórico:
 * 15/08/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

void Frame::binarizeImage(int threshold)
{
	int column = 0;
	int y = 0;

	// Percorro todo o frame coluna a coluna, pixel a pixel.
	for( column=0; column<this->getWidth(); column++ )
	{
		for( y=0; y<this->getHeight(); y++ )
		{
			// Se a luminancia for maior q o limiar, o pixel fica branco (é borda)
			if( this->getPixel(column,y) >= threshold )
				this->setPixel(column,y,255);
			else // senao fica preto (é fundo)
				this->setPixel(column,y,0);
		}
	}
}

/************************************************************************
 * Funcao que calcula a média de luminancia de uma imagem.
 *************************************************************************
 * param (E): Frame* frame => Imagem a ser calculada média.
 *************************************************************************
 * Histórico:
 * 18/08/08 - Ivan Shiguenori Machida
 * Criação.
 ************************************************************************/

double Frame::mediaBin()
{
	int x = 0;
	int y = 0;
	double mean=0;

	// Percorro todo o frame coluna a coluna, pixel a pixel.
	for( x=0; x<this->getWidth(); x++ )
	{
		for( y=0; y<this->getHeight(); y++ )
		{
			// Somaeoria de pixel do frame;
			mean += this->getPixel(x,y);
			//			Log::writeLog("%s :: mean[%.lf]", __FUNCTION__, mean);
		}
	}

	// Calcula-se a média dos pixels somados.
	mean = mean / (this->getWidth()*this->getHeight());

	//	Log::writeLog("%s :: mean-geral[%.lf]", __FUNCTION__, mean);

	return (mean);
}

/************************************************************************
 * Funcao que retorna a maior luminancia de um frame.
 *************************************************************************
 * param (E): Frame* frame => Frame do qual será retornada a maior 
 * 									  luminancia
 *************************************************************************
 * Histórico:
 * 27/08/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

int Frame::getMaxLum()
{

	int height = this->getHeight();
	int width = this->getWidth();
	int auxLum = 0;
	int maxLuminance = 0;

	for (int x=0; x<width; x++)
	{
		for (int y=0; y<height; y++)
		{
			auxLum = this->getPixel(x,y);
			if( auxLum > maxLuminance )
				maxLuminance = auxLum;
		}
	}

	return (maxLuminance);
}

int Frame::removeBorder()
{

	int height = 0;
	int width = 0;

	int sizeWide = 0;
	int minSize = 0;
	int y = 0;
	int x = 0;

	int maxLum = 0;

	height = this->getHeight();
	width = this->getWidth();

	Frame* frameAux = new Frame(this);

	maxLum = frameAux->getMaxLum();

	frameAux->binarizeImage(maxLum/4);

	Log::writeLog("%s :: height = %d, width = %d ", __FUNCTION__, height, width);

	for( x=0; x<height; x++)
	{	
		// Quando encontrar o pixel diferente de preto eu entro e guardo a altura.
		for( y=0; y<width ;y++)
		{
			//Log::writeLog("%s :: x[%d] y[%d]", __FUNCTION__, x , y);
			// Se o pixel for diferente de preto eu atribuo a altura do pixel como
			// sendo a altura do wide.
			if(frameAux->getPixel(y,x))
			{
				sizeWide = y;
				break;
			}
		}

		/**
		 * Se logo de cara encontrarmos um pixel não preto, significa que não tem Wide.
		 * Se a linha for toda preta sizeWide vai ser 0, mas não posso parar por causa disso,
		 * então só paro se sizeWide = 0 e a altura do wide for menor que a altura do frame.
		 **/
		if(!sizeWide && y<width)
			break;

		// Se minSize = 0, ainda não encontrou nenhum ponto não preto é porque ainda não encontrei
		// nenhuma altura candidata à altura do wide. Então atribuo a primeira que eu encontrar.
		if (!minSize) 
			minSize = sizeWide;

		/**
		 * Se a nova altura do wide encontrada for menor que a encontrada anteriormente, 
		 * atribuo esta como sendo a menor altura de wide. Caso contrário ignoro esta 
		 * nova altura.
		 * **/
		if (sizeWide < minSize )
			minSize = sizeWide;
		else
			sizeWide = minSize;

		// Se o tamanho do wide que achamos, for maior 
		// que metade da altura do frame, non ecziste
		if (sizeWide >= width/2)
			sizeWide = 0;
	}

	Log::writeLog("%s :: sizeWide = %d", __FUNCTION__, sizeWide);

	// Se houver widescreen
	if (sizeWide)
	{

		Log::writeLog("%s :: sizeWide_final = %d", __FUNCTION__, sizeWide);

		IplImage* img_dst;

		// Crio uma imagem nova com o tamanho do RV sem as faixas de widescreen
		img_dst = Frame::imgAlloc(cvSize(this->getWidth()-(sizeWide*2),this->getHeight()), this->data->depth, this->data->nChannels);

		// Pego somente a parte de interesse (sem o wide) do RV.
		cvSetImageROI(this->data,
				cvRect
				(
				 sizeWide,
				 0,
				 this->getWidth()-(sizeWide*2),
				 this->getHeight()
				)
				);

		// Copio para uma imagem nova
		imgCopy(this->data,img_dst);

		cvResetImageROI(this->data);

		// Seto o frame com o Ritmo Visual sem o wide 
		this->setImage(img_dst);

	}

	delete frameAux;

	return sizeWide;
}

/************************************************************************
 * Função que retira as tarjas widescreen do ritmo visual do vídeo.
 *************************************************************************
 * param (E): Frame* visualRythim : Ritmo visual a ser tratado
 *************************************************************************
 * return : Ritmo visual sem wide.
 *************************************************************************
 * Histórico:
 * 01/09/08 - Thiago Mizutani
 * Criação.
 ************************************************************************/

int Frame::removeWide()
{

	int height = 0;
	int width = 0;

	int sizeWide = 0;
	int minSize = 0;
	int y = 0;

	int maxLum = 0;

	height = this->getHeight();
	width = this->getWidth();

	Frame* frameAux = new Frame(this);

	maxLum = frameAux->getMaxLum();
	
	frameAux->binarizeImage(maxLum/4);

	Log::writeLog("%s :: height = %d, width = %d ", __FUNCTION__, height, width);

	for( int x=0; x<width; x++ )
	{	
		// Quando encontrar o pixel diferente de preto eu entro e guardo a altura.
		for( y=0; y<height; y++)
		{
			// Se o pixel for diferente de preto eu atribuo a altura do pixel como
			// sendo a altura do wide.
			if(frameAux->getPixel(x,y))
			{
				sizeWide = y;
				break;
			}
		}

		/**
		 * Se logo de cara encontrarmos um pixel não preto, significa que não tem Wide.
		 * Se a linha for toda preta sizeWide vai ser 0, mas não posso parar por causa disso,
		 * então só paro se sizeWide = 0 e a altura do wide for menor que a altura do frame.
		 **/
		if(!sizeWide && y<height)
			break;

		// Se minSize = 0, ainda não encontrou nenhum ponto não preto é porque ainda não encontrei
		// nenhuma altura candidata à altura do wide. Então atribuo a primeira que eu encontrar.
		if (!minSize) 
			minSize = sizeWide;

		/**
		 * Se a nova altura do wide encontrada for menor que a encontrada anteriormente, 
		 * atribuo esta como sendo a menor altura de wide. Caso contrário ignoro esta 
		 * nova altura.
		 * **/
		if (sizeWide < minSize )
			minSize = sizeWide;
		else
			sizeWide = minSize;

		// Se o tamanho do wide que achamos, for maior 
		// que metade da altura do frame, non ecziste
		if (sizeWide >= height/2)
			sizeWide = 0;
	}

	Log::writeLog("%s :: sizeWide = %d", __FUNCTION__, sizeWide);
	
	// Não existe wideScreen menor do que 10 pixels.
	if (sizeWide < 10)
		sizeWide = 0;
	
	// Se houver widescreen
	if (sizeWide)
	{

		Log::writeLog("%s :: sizeWide_final = %d", __FUNCTION__, sizeWide);

		IplImage* img_dst;

		// Crio uma imagem nova com o tamanho do RV sem as faixas de widescreen
		img_dst = Frame::imgAlloc(cvSize(width,(height-(sizeWide*2))), this->data->depth, this->data->nChannels);

		// Pego somente a parte de interesse (sem o wide) do RV.
		cvSetImageROI(this->data,
				cvRect
				(
				 0,
				 sizeWide,
				 width,
				 height-(sizeWide*2)
				)
				);

		// Copio para uma imagem nova
		imgCopy(this->data,img_dst);

		cvResetImageROI(this->data);

		// Seto o frame com o Ritmo Visual sem o wide 
		this->setImage(img_dst);

	}

	delete frameAux;

	return sizeWide;
}
