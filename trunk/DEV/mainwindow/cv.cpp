#include "cv.h"
#include "highgui.h"
#include <stdio.h>

IplImage* histogram(IplImage *src);
void genRV(IplImage *src, IplImage *rv);


#define MAX_OPTIONS 30
#define EFFECT_NAME_SIZE 30

#define HIST_HEIGHT 256
// O valor de largura tem que ser definido com cuidado
// a imgaem que vamos utilizar tem 256 niveis (cinza)
// mas no caso vamos imprimir para cada linha do histograma
// uma linha em branco, senão o histograma fica muito pequeno
#define HIST_WIDTH  256

int main( int argc, char** argv )
{
	char* imgname_cy;
	//char* videoname_cy = argc >= 3 ? argv[1] : (char*)"teste1.avi";
	char *effectName;
	char *effectsNameList[MAX_OPTIONS];
	IplImage *effectsImgList[MAX_OPTIONS];
	int effectCount = 0;

	IplImage* imgSrc = 0;
	IplImage* imgGray = 0;
	IplImage* imgEffect = 0;

	int i, j;
	int aux_i = 0;
	char aux_cy[30];

	memset(effectsNameList, '\0', sizeof(char*)*MAX_OPTIONS);
	memset(effectsImgList , '\0', sizeof(char*)*MAX_OPTIONS);


	if (argc > 1)
	{

		// Se a primeira opcao for -?, imprime o usage
		if (!strcmp(argv[1],"-?"))
		{
			usage();
			return -1;
		}

		if (argc > MAX_OPTIONS)
			printf("WARNING :: argc >= MAX_OPTIONS,  may trunk the output\n");

		// Se a primeira opcao da linha de comando jah for uma opcao de efeito
		// entao usa nome default de figura
		imgname_cy = argv[1][0] != '-' ? argv[1] : (char*)"teste1.jpg";

		if ( (imgSrc = cvLoadImage(imgname_cy,1)) == 0)
		{
			printf("Cannot open file: [%s]\n",imgname_cy);
			return -1;
		}

		// Se na linha de comando digitarem somente o nome da imagem, entao imprime ela
		if (argc == 2)
		{
			argv[2] = "-p";
			argc++;
		}

		// Para todos os filtros trabalhamos com imagens em escala de cinza
		// entao logo no comeco jah criamos nossa imagem base
		imgGray = cvCreateImage(cvGetSize(imgSrc),8,1);

		imgGray->origin  = imgSrc->origin;

		cvCvtColor(imgSrc, imgGray, CV_RGB2GRAY);

		for (i = 2 ; i < argc && effectCount < MAX_OPTIONS; i++)
		{
			if (argv[i][0] != '-')
				continue;

			// Cada efeito novo temos que alocar um espaco para o nome e para a imagem
			effectName = (char*)malloc(sizeof(char)*EFFECT_NAME_SIZE);
			memset(effectName, '\0', sizeof(effectName));

			imgEffect = cvCreateImage(cvGetSize(imgSrc),8,1);
			imgEffect->origin = imgSrc->origin;

			switch (argv[i][1])
			{
				case 't':

					aux_i = atoi(argv[++i]);

					sprintf(effectName, "Treshold t=%d", aux_i);
					cvThreshold(imgGray, imgEffect, aux_i, 255, CV_THRESH_BINARY_INV);

					break;
				case 'd':
					strcpy(effectName, "Dilate");
					cvDilate(imgGray, imgEffect, 0, 1);

					break;
				case 'e':
					strcpy(effectName, "Erode");
					cvErode(imgGray, imgEffect, 0, 1);
					break;

				case 'g':
					strcpy(effectName, "Gray");
					imgEffect = imgGray;

					break;
				case 's':

					strcpy(effectName, "Sobel");

					if (argv[i][2] == 'h')
					{
						// Aplica sobel horizontal
						strcat(effectName, " H");
						cvSobel(imgGray, imgEffect, 1, 0, 3);
					}
					else if (argv[i][2] == 'v')
					{
						// Aplica sobel vertical
						strcat(effectName, " V");
						cvSobel(imgGray, imgEffect, 0, 1, 3);
					}
					else
					{
						IplImage* imgAux = 0;
						imgAux = cvCreateImage(cvGetSize(imgSrc),8,1);

						// Aplica ambos (h/v)
						cvSobel(imgGray, imgAux, 1, 0, 3);
						cvSobel(imgAux, imgEffect, 0, 1, 3);
					}

					break;
				case 'h':
				case 'l':
					{
						if (argv[i][2] == 'p') // High/Low-Pass (Passa-baixa/alta)
						{
							IplImage* imgAux = 0;
							IplImage* imgSrcCpy = 0;

							// Tamanho da Matriz
							int cols_i = 3;
							int rows_i = 3;
							int kernel_i = 0;
							double *kernel;

							CvMat *filter = 0;
							imgSrcCpy = cvCloneImage(imgGray);

							if (argv[i][1] == 'h')
							{
								double kernel_high[][9] = {
									{
										-1, -1, -1,
										-1,  8, -1,
										-1, -1, -1,
									},
									{
										0, -1,  0,
										-1, 4, -1,
										0, -1,  0,
									},
									{
										1 , -2,  1,
										-2,  4, -2,
										1 , -2,  1,
									},
								};

								cols_i = 3;
								rows_i = 3;

								if (argc > i+1)
								{
									if (argv[i+1][0] >= '0' && ( argv[i+1][0] <= '3' ))
										kernel_i = atoi(argv[++i]) -1;
								}

								kernel = kernel_high[kernel_i];

								sprintf(effectName, "High-Pass kernel [%d]", kernel_i+1);
							}
							else
							{
								double kernel_low[][25] = { 
									{
										1.0/25.0, 1.0/25.0, 1.0/25.0, 1.0/25.0, 1.0/25.0,
										1.0/25.0, 1.0/25.0, 1.0/25.0, 1.0/25.0, 1.0/25.0,
										1.0/25.0, 1.0/25.0, 1.0/25.0, 1.0/25.0, 1.0/25.0,
										1.0/25.0, 1.0/25.0, 1.0/25.0, 1.0/25.0, 1.0/25.0,
										1.0/25.0, 1.0/25.0, 1.0/25.0, 1.0/25.0, 1.0/25.0,
									},
								};

								cols_i = 5;
								rows_i = 5;

								if (argc > i+1)
								{
									if (argv[i+1][0] >= '0' && ( argv[i+1][0] <= '1' ))
										kernel_i = atoi(argv[++i]) - 1;
								}

								kernel = kernel_low[kernel_i];

								sprintf(effectName, "Low-Pass kernel [%d]", kernel_i+1);
							}



							imgAux = cvCreateImageHeader(cvGetSize(imgSrcCpy), 8, 1);
							imgAux->imageData = imgSrcCpy->imageData;
							imgAux->widthStep = imgSrcCpy->width;

							imgEffect = cvCreateImageHeader(cvGetSize(imgSrcCpy), 8, 1);
							imgEffect->imageData = imgAux->imageData;
							imgEffect->widthStep = imgAux->width;

							filter = cvCreateMatHeader(rows_i, cols_i, CV_64FC1);

							cvSetData(filter, kernel, cols_i*8);

							cvFilter2D(imgAux, imgEffect, filter, cvPoint(-1,-1));

						}
						else // Histograma
						{

							// O histograma tem um esquema diferente, pois ele nao
							// tem uma imagem do tamanho da imgSrc, entao faremos 
							// um processo diferente

							IplImage* imgAux = 0;

							strcpy(effectName, "Histogram of");

							// Se ja tiver aplicado algum efeito, tiramos o histograma desta nova imagem
							// somente se ela NAO for a imagem de entrada ( que tem 8 canais, e no histograma
							// esperamos uma imagem de apenas 1 canal )
							if (effectCount >= 1 && strcmp(effectsNameList[effectCount-1],imgname_cy))
							{

								imgAux = histogram(effectsImgList[effectCount-1]);
								printf("%s :: Histogram from [%s]\n", __FUNCTION__, effectsNameList[effectCount-1]);
								sprintf(effectName, "%s %s", effectName, effectsNameList[effectCount-1]);
							}
							else
							{
								imgAux = histogram(imgGray);
								printf("%s :: Histogram from [%s]\n", __FUNCTION__, imgname_cy);
								sprintf(effectName, "%s %s", effectName, imgname_cy);
							}

							printf("%s :: Effect : [%s]\n", __FUNCTION__, effectName);

							// Adiciona na listas
							effectsNameList[effectCount] = effectName;
							effectsImgList[effectCount] = imgAux;

							effectCount++;


							cvNamedWindow(effectName, 1);

							cvShowImage(effectName, imgAux);

							continue;
						}

					}
					break;
				case 'w':
					if (effectCount >= 1)
					{
						char filename_cy[50];

						strcpy(filename_cy, imgname_cy);
						// Tira a extensao
						filename_cy[strlen(filename_cy) - 4] = '\0';

						sprintf(filename_cy, "%s_%s.png", filename_cy, effectsNameList[effectCount-1]);

						printf("%s :: Writing effet [%s] in file [%s]\n", __FUNCTION__, effectsNameList[effectCount-1], filename_cy);

						if(!cvSaveImage(filename_cy,effectsImgList[effectCount-1]))
						{
							printf("Could not save: %s\n",filename_cy);
						}
					}

					continue;

				case 'p':
					strcpy(effectName, imgname_cy);
					imgEffect = imgSrc;
					break;

				case '?':
				default:
					usage();
					return -1;

					break;

			}

			printf("%s :: Effect : [%s]\n", __FUNCTION__, effectName);

			effectsNameList[effectCount] = effectName;
			effectsImgList[effectCount] = imgEffect;

			effectCount++;

			cvNamedWindow(effectName, 1);
			cvShowImage(effectName, imgEffect);
		}

		if (effectCount >= MAX_OPTIONS)
			printf("WARNING :: More effects than expected, truncating (max_options = %d)",MAX_OPTIONS);

		cvWaitKey(0);
	}
	else
	{
		usage();
		return -1;
	}

	for (i=0 ; i < effectCount ; i++)
	{
		printf("%s :: Destroy [%s]\n", __FUNCTION__, effectsNameList[i]); 
		cvDestroyWindow(effectsNameList[i]);
		free(effectsNameList[i]);
	}

	return 0;
}

IplImage* histogram(IplImage *src)
{
	// Tamanho de um histograma 1D
	int bins = HIST_WIDTH;
	int hsize[] = {bins};

	IplImage* imgHistogram = 0;

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
	printf("%s :: min: [%f], max: [%f]\n",__FUNCTION__,min_value, max_value);

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

		//printf("%s :: i[%d], value[%-11f], normalized[%-8f]\n", __FUNCTION__, i, value, normalized);

		// Printa a linha do Histograma
		cvLine(imgHistogram, cvPoint((i*2)-1,HIST_HEIGHT), cvPoint((i*2)-1,HIST_HEIGHT-normalized), CV_RGB(0, 0, 0));

		// Print uma linha em branco, pro histograma nao ficar todo grudado
		cvLine(imgHistogram, cvPoint(i*2,HIST_HEIGHT), cvPoint(i*2,HIST_HEIGHT-normalized), CV_RGB(0, 255, 255));
	}

	return imgHistogram;
}

void genRV(IplImage *src, IplImage *rv)
{
	memset(rv, sizeof(rv->nSize), '\0');

	int y=0;
	int x=0;

	// Varre por largura
	for (y=0 ; y <= src->height; y++)
	{
		printf("%s :: x[%d], y[%d]\n", __FUNCTION__, x ,y);
		for (x=0 ; x <= y ; x++)
		{
			(rv->imageData + rv->widthStep*y)[x] = \
																(src->imageData + src->widthStep*y)[x];
		}
	}
}

