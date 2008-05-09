#include "cv.h"
#include "highgui.h"
#include <stdio.h>

IplImage* histogram(IplImage *src);
void genRV(IplImage *src, IplImage *rv);

IplImage* imgHistogram = 0;

#define HIST_HEIGHT 256
// O valor de largura tem que ser definido com cuidado
// a imgaem que vamos utilizar tem 256 niveis (cinza)
// mas no caso vamos imprimir para cada linha do histograma
// uma linha em branco, senão o histograma fica muito pequeno
#define HIST_WIDTH  256

int main( int argc, char** argv )
{
	char* imgname_cy = argc >= 2 ? argv[1] : (char*)"teste1.jpg";
	char* videoname_cy = argc >= 3 ? argv[1] : (char*)"teste1.avi";
	IplImage* img0 = 0;
	IplImage* imgGray = 0;
	IplImage* imgErode = 0;
	IplImage* imgSobelH = 0;
	IplImage* imgSobelV = 0;
	IplImage* imgSobel = 0;
	IplImage* imgHist = 0;
	IplImage* imgRV = 0;

	if ( (img0 = cvLoadImage(imgname_cy,1)) == 0)
		printf("Cannot open file: [%s]\n",imgname_cy);

	cvNamedWindow("ImageGray", 1);
	cvNamedWindow("ImageRV", 1);
	//cvNamedWindow("ImageErode", 1);
	//cvNamedWindow("ImageSobelH", 1);
	//cvNamedWindow("ImageSobelV", 1);
	//cvNamedWindow("ImageSobel", 1);
	cvNamedWindow("Histogram", 1);

	imgGray  = cvCreateImage(cvGetSize(img0),8,1);
	imgErode = cvCreateImage(cvGetSize(img0),8,1);
	imgSobel = cvCreateImage(cvGetSize(img0),8,1);
	imgSobelH = cvCreateImage(cvGetSize(img0),8,1);
	imgSobelV = cvCreateImage(cvGetSize(img0),8,1);
	imgRV = cvCreateImage(cvGetSize(img0),8,1);

	// Para a imagem nao ficar de ponta cabeca
	imgGray->origin  = img0->origin;
	imgErode->origin = img0->origin;

	// Converte pra cinza
	cvCvtColor(img0, imgGray, CV_RGB2GRAY);

	// Cria o histograma
	imgHist = histogram(imgGray);

	// Dilata a imagem
	cvErode(imgGray, imgErode, 0, 1);

	// Aplica sobel horizontal
	cvSobel(imgGray, imgSobelH, 1, 0, 3);

	// Aplica sobel vertical
	cvSobel(imgGray, imgSobelV, 0, 1, 3);

	// Aplica ambos (h/v)
	cvSobel(imgSobelH, imgSobel, 0, 1, 3);

	// Gera o ritmo visual
	genRV(imgGray, imgRV);

	//cvShowImage("Image", img0);
	/*
	cvShowImage("ImageErode", imgErode);
	cvShowImage("ImageSobelH", imgSobelH);
	cvShowImage("ImageSobelV", imgSobelV);
	cvShowImage("ImageSobel", imgSobel);
	*/
	cvShowImage("ImageGray" , imgGray);
	cvShowImage("ImageRV"   , imgRV);
	cvShowImage("Histogram" , imgHist);

	cvWaitKey(0);

	cvDestroyWindow("ImageGray");
	/*
	cvDestroyWindow("ImageErode");
	cvDestroyWindow("ImageSobelH");
	cvDestroyWindow("ImageSobelV");
	//cvDestroyWindow("Imagem");
	*/
	cvDestroyWindow("imageRV");
	cvDestroyWindow("Histogram");

	return 0;
}

IplImage* histogram(IplImage *src)
{
	// Tamanho de um histograma 1D
	int bins = HIST_WIDTH;
	int hsize[] = {bins};

	CvHistogram* hist = 0;

	float max_value = 0;
	float min_value = 0;

	float value;
	int normalized;

	// Ranges, escala de cinza: 0-256
	float xranges[] = {0, 256};
	float* ranges[] = {xranges};

	int i;

	IplImage* planes[] = {src};

	hist = cvCreateHist(1, hsize, CV_HIST_ARRAY, ranges, 1);
	cvCalcHist(planes, hist, 0, NULL);
	cvGetMinMaxHistValue(hist, &min_value, &max_value);
	printf("min: [%f], max: [%f]\n",min_value, max_value);

	if (imgHistogram == NULL)
		imgHistogram = cvCreateImage(cvSize(bins*2, HIST_HEIGHT), 8, 1);

	// Desenha a area de trabalho do histograma
	cvRectangle(imgHistogram, cvPoint(0,0), cvPoint(HIST_WIDTH*2,HIST_HEIGHT), CV_RGB(255,255,255), -1);

	// Desenhar as linhas do histograma
	for (i=1 ; i <= bins ; i++)
	{
		// Pega o valor do histograma na posicao i da imagem
		value = cvQueryHistValue_1D(hist, i-1);
		normalized = cvRound(value*HIST_HEIGHT/max_value);

		printf("i[%d], value[%-11f], normalized[%-8f]\n", i, value, normalized);

		// Printa a linha do Histograma
		cvLine(imgHistogram, cvPoint((i*2)-1,HIST_HEIGHT), cvPoint((i*2)-1,HIST_HEIGHT-normalized), CV_RGB(0, 0, 0));

		// Print uma linha em branco, pro histograma nao ficar todo grudado
		cvLine(imgHistogram, cvPoint(i*2,HIST_HEIGHT), cvPoint(i*2,HIST_HEIGHT-normalized), CV_RGB(0, 255, 255));
	}

	return (imgHistogram);
}

void genRV(IplImage *src, IplImage *rv)
{
	memset(rv, sizeof(rv->nSize), '\0');
	
	int y=0;
	int x=0;

	// Varre por largura
	for (y=0 ; y <= src->height; y++)
	{
		printf("x[%d], y[%d]\n",x ,y);
		for (x=0 ; x <= y ; x++)
		{
			(rv->imageData + rv->widthStep*y)[x] = \
				(src->imageData + src->widthStep*y)[x];
		}
	}
}

/*
	char c;
	CvCapture* capture = 0;
	IplImage* frame = 0;
	IplImage* framegray = 0;
	IplImage* framecanny = 0;
	IplImage* framesmooth = 0;
	IplImage* framedilate = 0;

	printf("Opening %s\n",argv[1]);

	capture = cvCaptureFromAVI(argv[1]); 

	cvNamedWindow("Imagem", 1);
	cvNamedWindow("ImagemGray", 1);
	cvNamedWindow("ImagemCanny", 1);
	cvNamedWindow("ImagemSmooth", 1);
	cvNamedWindow("ImagemDilate", 1);

	if (!capture)
	{
		printf("File not found\n");
		return -1;
	}

	while(true)
	{
		frame = cvQueryFrame(capture);

		if (!frame)
			break;

		// Todas imagens: 8 bit por pixel, 1 canal ( RGB tem 3 ! )
		if (!framegray)
			framegray = cvCreateImage(cvGetSize(frame),8,1);

		if (!framesmooth)
			framesmooth = cvCreateImage(cvGetSize(frame),8,1);

		if (!framecanny)
			framecanny = cvCreateImage(cvGetSize(frame),8,1);

		if (!framedilate)
			framedilate = cvCreateImage(cvGetSize(frame),8,1);

		framegray->origin = frame->origin;
		cvCvtColor(frame, framegray, CV_RGB2GRAY);
		framecanny->origin = frame->origin;
		framesmooth->origin = frame->origin;
		framedilate->origin = frame->origin;

		cvCvtColor(frame, framegray, CV_RGB2GRAY);

		cvCanny(framegray, framecanny, 30, 80, 3);

		cvSmooth(framegray, framesmooth, CV_GAUSSIAN, 15, 0, 0, 0);

		cvDilate(framecanny, framedilate, 0, 1);


		cvShowImage("Imagem", frame);
		cvShowImage("ImagemGray", framegray);
		cvShowImage("ImagemCanny", framecanny);
		cvShowImage("ImagemSmooth", framesmooth);
		cvShowImage("ImagemDilate", framedilate);

		c = cvWaitKey(30);
		printf("Key -> %c | %d\n",c,c);

		// 27 = ESC
		if ( (char) c == 27 )
			break;

	}

	cvWaitKey(10);

	cvDestroyWindow("Imagem");
	cvDestroyWindow("ImagemGray");
	cvDestroyWindow("ImagemCanny");
	cvDestroyWindow("ImagemSmooth");
	cvDestroyWindow("ImagemDilate");

	cvReleaseImage(&frame);
	cvReleaseImage(&framegray);
	cvReleaseImage(&framecanny);
	cvReleaseImage(&framesmooth);
	cvReleaseCapture(&capture);

	return -1;

}

	IplImage* img;

	img = cvLoadImage(argv[1], 1);

	cvNamedWindow("Imagem", 1);
	cvShowImage("Imagem", img);
	cvWaitKey(0);

	return -1;
*/
