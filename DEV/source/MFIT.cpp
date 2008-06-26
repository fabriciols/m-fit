#include "../include/Filters.h"
//#include "../include/Morphology.h"

#include "highgui.h"

#include <stdio.h>
#include <stdlib.h>

#define EFFECT_NAME_SIZE 30
#define MAX_OPTIONS 30

void usage();

int main(int argc, char* argv[])
{

	// Frame original
	Frame *frame;
	// Frame original em escala de cinza
	Frame *frameGray;
	// Frame original com o efeito aplicado
	Frame *frameEffect;
	// Nome do arquivo
	char filename_cy[20];
	// Nome do efeito aplicado
	char *effectName;

	// Classe que aplica os filtros
	Filters filters;

	// Classe que aplica os filtros de morfologia matematica
	//Morphology morph;

	int i, aux_i, effectCount = 0;

	struct 
	{
		Frame *frame;
		char  *name ;
	} effectsList[MAX_OPTIONS];
	

	// Se nao mandarem o nome do arquivo como primeiro parametro
	// adota default
	if (argc > 1)
	{
		strcpy(filename_cy, argv[1]);
	}
	else
	{
		strcpy(filename_cy, "teste.jpg");
	}

	// Tenta abrir o arquivo
	try
	{
		frame = new Frame(filename_cy);
	}
	catch (char *str)
	{
		printf("Exception: %s\n", str);
		return 1;
	}
	catch (...)
	{
		printf("Unhandled exception\n");
		return 1;
	}

	// Todos os tratamentos usamos img em cinza, então vamos 
	// transformar nosso frame em cinza.
	frameGray = frame->convert2Gray();

	// Faço a varredura nos parametros passados pela linha de comando
	for (i = 2 ; i < argc ; i++)
	{

		if (argv[i][0] != '-')
			continue;

		// Cria uma nova string para colocar na lista de efeitos
		effectName = new char[ sizeof(char)*EFFECT_NAME_SIZE ];

		switch (argv[i][1])
		{
			case 't':

				aux_i = atoi(argv[++i]);

				sprintf(effectName, "Treshold t=%d", aux_i);

				frameEffect = filters.segment(frameGray, aux_i);

				break;
			case 'd':
				strcpy(effectName, "Dilate");

				//frameEffect = morph.dilate(frame_gray);

				break;
			case 'e':
				strcpy(effectName, "Erode");

				//frameEffect = morph.erode(frame_gray);

				break;
         case 's':       
                
            if (argv[i][2] == 'v')
            {
               strcpy(effectName, "Vertival Sobel");
               frameEffect = filters.Sobel(frameGray, 0);
            }
				else if (argv[i][2] == 'h')
            {
               strcpy(effectName, "Horizontal Sobel");
               frameEffect = filters.Sobel(frameGray, 1);               
            }
				else
            {
               strcpy(effectName, "Complete Sobel");
               frameEffect = filters.Sobel(frameGray, 2);
            }
            
            break;
         
         case 'l':

				strcpy(effectName, "Low-Pass Filter");
				
				//Se for passado algum argumento como valor para tamanho da máscara
				//será = tamanho passado, senão assume por default o valor 5.
				if (('0' <= atoi(argv[++i]) <= '9') && (i <= argc))	
					aux_i = atoi(argv[i]); //Passo i porque já somei 1 e estou na posição desejada
				else
					aux_i = 5;

				frameEffect = filters.lowPass(frameGray, aux_i); 
            
				break;
				
				/*
			case 'g':
				strcpy(effectName, "Gray");
				imgEffect = imgGray;

				break;
			
			case 'h':
			case 'l':
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
				else if (argv[i][1] == 'h') // Histograma
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

			*/
			case 'p':
				strcpy(effectName, filename_cy);
				frameEffect = frame;
				break;

			case '?':
			default:
				usage();
				return -1;
				break;

		}

		printf("Effect[%d] : [%s]\n", effectCount, effectName);

		// Salva os dados na estrutura, para poder aplicar efeitos na imagem anterior
		effectsList[effectCount].name  = effectName;
		effectsList[effectCount].frame = frameEffect;

		// Imprime na tela
		cvNamedWindow(effectsList[effectCount].name, 1);
		cvShowImage(effectsList[effectCount].name, effectsList[effectCount].frame->data);

		effectCount++;

	}

	cvWaitKey(0);

	// Limpa a memória
	for (i=0 ; i < effectCount ; i++)
	{
		printf("Destroy[%d]: [%s]\n", i, effectsList[i].name); 

		cvDestroyWindow(effectsList[i].name);
		delete effectsList[i].name;
		delete effectsList[i].frame;
	}

}

void usage()
{
	int i;
	char *usage_cy[] = {
		"Use: MFIT.exe img_src -[t|d|e|s|h]  Apply filter in img_src\n",
		"MFIT -t T  Segmentation of the image with the threshold T",
		"MFIT -d    Dilate the image",
		"MFIT -e    Erode the image",
		"MFIT -sh   Apply Sobel (Horizontal) filter in the image",
		"MFIT -sv   Apply Sobel (Vertial) filter in the image",
		"MFIT -s    Apply Sobel (Both) filter in the image",
		"MFIT -h    Print the histogram of the last img",
		"MFIT -p    Print the img_src",
		"MFIT -g    Print the img_src in gray",
		"MFIT -lp M Apply low-pass filter with the kernel M",
		"MFIT -hp M Apply high-pass filter with the kernel M",
		"MFIT -w    Write the last effect on a file",
		""
	};

	for (i=0 ; usage_cy[i][0] != '\0' ; i++)
	{
		printf("%s\n", usage_cy[i]);
	}
}
