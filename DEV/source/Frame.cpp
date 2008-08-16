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
* deve ser usada toda vez
*************************************************************************
* param (E): Nenhum
*************************************************************************
* Hist�rico:
* 29/07/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
void Frame::setImage(IplImage* imgNew)
{
	// Nao esta funcionando
	// precisa dar uma entendida o por que
	if (this->data != NULL)
	{
		// Desaloca
		imgDealloc(this->data);
	}

	// Aponta para a img nova
	this->data = imgNew;

	// Aponta para os valores do IplImage
	this->width =  &this->data->width;
	this->height = &this->data->height;

	Log::writeLog("%s :: new: this->data[%x]", __FUNCTION__, imgNew);
}

/************************************************************************
* Inicializa todos os atributos com nulo
* Esta fun��o deve ser usada em TODOS os construtores
*************************************************************************
* param (E): Nenhum
*************************************************************************
* Hist�rico:
* 29/07/08 - Fabricio Lopes de Souza
* Cria��o.
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
* Hist�rico:
* 29/07/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Frame::Frame()
{
	initAttr();
}

/************************************************************************
* Construtor para Frame que recebe os valores necess�rios para plotar
* um histograma. Cria um objeto Frame com o desenho do histograma.
*************************************************************************
* param (E): double matrix[] - Valores do Histograma
* param (E): int len_i       - Quantidade de valores em 'matrix'
* param (E): float max_f     - Valor m�ximo do histograma
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Frame::Frame(double *matrix, int len_i, float max_f)
{

	int i;
	int normalized;
	char *frame_cy;
	CvFont font;

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

		Log::writeLog("%s :: Plot Value[%3d] = [%4.0lf] Normalized = [%3d]", __FUNCTION__, i, matrix[i], normalized);

		// Printa a linha do Histograma
		//cvLine(imgHistogram, cvPoint((i*2)-1,HIST_HEIGHT), cvPoint((i*2)-1,HIST_HEIGHT-normalized), CV_RGB(0, 0, 0));
		cvLine(imgHistogram, cvPoint((i)-1,HIST_HEIGHT), cvPoint((i)-1,HIST_HEIGHT-normalized), CV_RGB(0, 0, 0));

		if (i % 32 == 0)
		{
			sprintf(frame_cy, "%d", i);
			Log::writeLog("%s :: i[%d] == cvPutText :[%s] cvPoint(%d,%d)", __FUNCTION__, i, frame_cy, i-1, HIST_HEIGHT);
			cvPutText(imgHistogram, frame_cy, cvPoint(i-1,10), &font, CV_RGB(0,0,0));
			cvLine(imgHistogram, cvPoint((i)-1,0), cvPoint((i)-1,HIST_HEIGHT), CV_RGB(0, 0, 0));
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
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
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
* Construtor para Frame que recebe uma imagem j� carregada
* faz uma copia dessa imagem, e deixa o endereco no frame.
* Cria um Frame com base nos dados da imagem passada.
*************************************************************************
* param (E): IplImage* img_src - Imagem a ser usada como base para a
* cria��o de um Frame
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
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
* Construtor para Frame que recebe um frame j� instanciado.
* Clona o frame.
*************************************************************************
* param (E): Frame *frame - Frame a ser clonado.
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
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
* Fun��o que retorna a largura do frame.
*************************************************************************
* param (E): Nenhum.
*************************************************************************
* return : int - Largura do frame.
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
int Frame::getWidth()
{
	return(*this->width);
}

/************************************************************************
* Fun��o que retorna a altura do frame.
*************************************************************************
* param (E): Nenhum.
*************************************************************************
* return : int - Altura do frame. 
*************************************************************************
* Hist�rico:
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
int Frame::getHeight()
{
	return(*this->height);
}

/************************************************************************
* FUN��O QUE PEGA A DIAGONAL DE UM FRAME. 
*************************************************************************
* Esta fun��o utiliza as f�rmulas de trigonometria para encontrar a equa
* ��o da reta a partir disto obter todos os pontos da diagonal do frame. 
*************************************************************************
* param (E): Nenhum
*************************************************************************
* return : Frame* -> Frame com a diagonal
*************************************************************************
* Hist�rico:
* 29/07/08 - Fabr�cio Lopes de Souza
* 'N' altera��es para fazer funcionar, precisa-se validar com perfei��o
* para verificar se est� correto.
* 15/07/08 - Thiago Mizutani
* Adequa��o do processo de montagem da diagonal do frame. Utilizando as
* fun��es de getPixel e set Pixel.
* 06/07/08 - Thiago Mizutani
* Ajustes para extracao dos pontos da diagonal principal do frame.
* 04/07/08 - Thiago Mizutani
* Cria��o.
************************************************************************/
Frame * Frame::getDiagonal()
{
	int x = 0; // x da equacao da reta
	int y = 0; // y da equacao da reta

	Frame* frameDiagonal;

	int luminance = 0; //Valor de luminancia do pixel retirado da diagonal.
	
	double a = 0; // Coeficiente angular da equacao

	Log::writeLog("%s :: diagonal img width[%d] height[%d]", __FUNCTION__, 1, this->getWidth());

	IplImage* imgDiagonal = imgAlloc(cvSize(1, this->getWidth()), 8, 1);

	frameDiagonal = new Frame(imgDiagonal);

	imgDealloc(imgDiagonal);

	/** Calculo o coeficiente angular da reta ('a' da equacao).
	 * this->getHeight = y - yo
	 * this->getWidth  = x - xo
	 * y - yo = m*(x - xo)
	 **/
	a = (float)(this->getHeight()/(float)this->getWidth());

	// Pego a diagonal do this.
	for (x=0; x < this->getWidth() ; x++)
	{
		y = cvRound(a * x);

		// Monto o this da diagonal (j� transformado em coluna).
		frameDiagonal->setPixel(0, x, this->getPixel(x, y));
	}	


	return(frameDiagonal);

}

/************************************************************************
 * Fun��o que define a luminancia de um pixel
 *************************************************************************
 * param (E): int x   -> coordenada x do pixel desejado
 * param (E): int y   -> coordenada y do pixel desejado
 * param (E): int lum -> valor de luminancia a ser definido no pixel (x,y)
 *************************************************************************
 * Hist�rico:
 * 14/07/08 - Fabricio Lopes de Souza
 * Cria��o.
 ************************************************************************/
void Frame::setPixel(int x, int y, int lum)
{
	CvScalar s;

	s.val[0] = lum;

	cvSet2D(this->data, y, x, s);
}

/************************************************************************
 * Fun��o que retorna o valor de luminancia de um determinado pixel (x,y)
 *************************************************************************
 * param (E): int x   -> coordenada x do pixel desejado
 * param (E): int y   -> coordenada y do pixel desejado
 *************************************************************************
 * Hist�rico:
 * 14/07/08 - Fabricio Lopes de Souza
 * Cria��o.
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
* Hist�rico:
* 28/07/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
Frame::~Frame()
{
	// Se o ponteiro para a imagem nao for nulo
	if (this->data)
	{
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
* Hist�rico:
* 28/07/08 - Fabricio Lopes de Souza
* Cria��o.
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

	// ent�o copio esta imagem para esta �rea
	imgCopy(this->data,imgDst);

	Log::writeLog("%s :: frame x[%d] y[%d] width[%d] height[%d]", __FUNCTION__, this->getWidth(), 0, frame.getWidth(), frame.getHeight());

	// Agora, seto a area de interesse o espaco que a segunda imagem ir� ocupar
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

	// Removo as �reas de interesse da imagem
	cvResetImageROI(imgDst);

	// Seta pra imagem nova
	setImage(imgDst);

	Log::writeLog("%s :: this->data[%x]", __FUNCTION__, this->data);

}

/************************************************************************
* Fun��o que chama o construtor do histograma e retorna o histograma
* do frame. 
*************************************************************************
* param (E): IplImage* frame => Frame do qual ser� gerado o histograma
* param (S): Histogram* &histogram => Histograma do frame de entrada
*************************************************************************
* Hist�rico:
* 29/07/08 - Thiago Mizutani
* Cria��o.
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
* Hist�rico:
* 05/08/08 - Fabricio Lopes de Souza
* Cria��o.
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
 * Hist�rico:
 * 05/08/08 - Fabricio Lopes de Souza
 * Cria��o.
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
* Hist�rico:
* 12/08/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
void Frame::imgCopy(IplImage *imgSrc, IplImage *imgDst)
{
	cvCopy(imgSrc, imgDst);

	imgDst->origin = imgSrc->origin;
}

/************************************************************************
* Funcao que centraliza a alocacao de memoria e criacao de uma imagem
*************************************************************************
* param (E): CvSize  size     -> Altura e largura da imagem
* param (E): int		depth    -> Quantidade de bits para as cores
* param (E): int		channels -> Quantidades de canais possiveis
*************************************************************************
* Hist�rico:
* 12/08/08 - Fabricio Lopes de Souza
* Cria��o.
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
* Hist�rico:
* 12/08/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
void Frame::imgDealloc(IplImage *img)
{
	Log::writeLog("%s :: Delete image: [%x]", __FUNCTION__, img);

	cvReleaseImage(&img);
}

/************************************************************************
* Funcao que realiza a binariza��o de uma imagem. Tudo aquilo que
* possuir luminancia menor do que o limiar passado, ter� seu valor 
* alterado para 0 e tudo aquilo que tiver luminancia acima do valor do 
* limiar ter� seu valor alterado para 255.
*************************************************************************
* param (E): Frame* frame => imagem a ser binarizada.
* param (E): Int threshold => Limiar para binariza��o
*************************************************************************
* Hist�rico:
* 15/08/08 - Thiago Mizutani
* Cria��o.
************************************************************************/

Frame* Frame::binarizeImage(Frame* frame, int threshold)
{
	int column = 0;
	int y = 0;

	// Percorro todo o frame coluna a coluna, pixel a pixel.
	for( column=0; column<frame->getWidth(); column++ )
	{
		for( y=0; y<frame->getHeight(); y++ )
		{
			// Se a luminancia for maior q o limiar, o pixel fica branco (� borda)
			if( frame->getPixel(column,y) >= threshold )
				frame->setPixel(column,y,255);
			else // senao fica preto (� fundo)
				frame->setPixel(column,y,0);
		}
	}

	return (frame);
}