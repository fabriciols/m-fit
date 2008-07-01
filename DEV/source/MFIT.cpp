#include "cv.h"

#include "../include/Log.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"

#include "../include/Effect.h"
#include "../include/Color.h"

#include "../include/Filters.h"
#include "../include/Morphology.h"

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

	// Objeto de efeitos de cor
	Color *color;

	// Nome do arquivo
	char filename_cy[20];
	// Nome do efeito aplicado
	char *effectName;

	// Classe que aplica os filtros
	//Filters filters;

	// Classe que aplica os filtros de morfologia matematica
	//Morphology morph;

	int i, aux_i, effectCount = 0;

	// Estrutura para armazenar a lista de efeitos aplicados
	// possibilitando assim aplicar efeitos sobre efeitos,
	// e aumentando a memória usada :)
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
		Log::writeLog("%s :: new Frame filename[%s]", __FUNCTION__, filename_cy);
		frame = new Frame(filename_cy);
	}
	catch (char *str)
	{
		Log::writeLog("%s :: Exception [%s]", __FUNCTION__, str);
		usage();
		return 1;
	}
	catch (...)
	{
		Log::writeLog("%s :: Unhandled Exception", __FUNCTION__);
		usage();
		return 1;
	}

	// Todos os tratamentos usamos img em cinza, então vamos 
	// transformar nosso frame em cinza.

	color = new Color();

	frameGray = color->convert2Gray(frame);

	// Faço a varredura nos parametros passados pela linha de comando
	for (i = 2 ; i < argc ; i++)
	{

		if (argv[i][0] != '-')
			continue;

		Log::writeLog("%s :: param: argv[%d] = [%s]", __FUNCTION__, i, argv[i]);

		// Cria uma nova string para colocar na lista de efeitos
		effectName = new char[ sizeof(char)*EFFECT_NAME_SIZE ];

		// Zera a variável auxiliar
		aux_i = 0;

		switch (argv[i][1])
		{
			case 't':
				{

					Filters *filters = new Filters();

					aux_i = atoi(argv[++i]);

					sprintf(effectName, "Treshold t=%d", aux_i);

					frameEffect = filters->segment(frameGray, aux_i);

					break;

				}
			case 'd':
				{
					Morphology *morph = new Morphology();

					strcpy(effectName, "Dilate");

					frameEffect = morph->dilate(frameGray);

					break;
				}
			case 'e':
				{

					Morphology *morph = new Morphology();

					strcpy(effectName, "Erode");

					frameEffect = morph->erode(frameGray);

					break;

				}
			case 's':       
				{

					Filters *filters = new Filters();

					if (argv[i][2] == 'v')
					{
						strcpy(effectName, "Vertival Sobel");
						frameEffect = filters->Sobel(frameGray, 0);
					}
					else if (argv[i][2] == 'h')
					{
						strcpy(effectName, "Horizontal Sobel");
						frameEffect = filters->Sobel(frameGray, 1);               
					}
					else
					{
						strcpy(effectName, "Complete Sobel");
						frameEffect = filters->Sobel(frameGray, 2);
					}

					break;
				}

			case 'l':
				{

					Filters *filters = new Filters();

					strcpy(effectName, "Low-Pass Filter");

					//Se for passado algum argumento como valor para tamanho da máscara
					//será = tamanho passado, senão assume por default o valor 5.
					if ( i < argc )
					{
						if (argv[i+1][0] <= '9' && argv[i+1][0] >= '0')
						{
							Log::writeLog("%s :: LowPass param[%s]", __FUNCTION__, argv[i+1]);

							aux_i = atoi(argv[++i]); //Passo o valor do proximo parametro passado
						}
					}

					// 5 é o padrão
					if (!aux_i)
					{
						aux_i = 5;
					}

					frameEffect = filters->lowPass(frameGray, aux_i); 

					break;
				}

			case 'H':
				{
					Filters *filters = new Filters();
					// Se o usuário não escolher o tipo de matriz a ser usada o sistema adota
					// uma como padrão.

					if (('0' <= atoi(argv[i+1]) <= '9') && (i <= argc))	
					{
						aux_i = atoi(argv[++i]); // Incrementa i para que a proxima analise do for não pegue o mesmo parametro
					}
					else
					{
						aux_i = 1;
					}

					sprintf(effectName, "High-Pass kernel [%d]", aux_i);
					frameEffect = filters->highPass(frameGray, aux_i);
					break;
				}
			case 'h':
				{

					Frame *frameAux;

					// Se ja foi aplicado algum tipo de efeito, realizamo o histograma do ultimo efeito aplicado
					if (effectCount >= 1)
					{

						frameAux = effectsList[effectCount-1].frame;

						sprintf(effectName, "Histogram of %s", effectsList[effectCount-1].name);

						Log::writeLog("%s :: Histogram from effect[%d] [%s]\n",
								__FUNCTION__, effectCount-1, effectsList[effectCount-1].name);
					}
					else // Senão, aplicamos na imagem original mesmo
					{
						frameAux = frameGray;
						sprintf(effectName, "Histogram of %s", filename_cy);
					}

					Histogram *hist = new Histogram(frameAux->data);

					Log::writeLog("%s :: min[%d]: [%.0f], max[%d]: [%.0f]",
							__FUNCTION__, hist->getMinIdx(), hist->getMin(), hist->getMaxIdx(), hist->getMax());

					frameEffect = new Frame(hist->getMatrix(), 256, hist->getMax());

					break;
				}

			case 'w':
				if (effectCount >= 1)
				{
					char imgname_cy[50];

					strcpy(imgname_cy, filename_cy);
					// Tira a extensao
					imgname_cy[strlen(imgname_cy) - 4] = '\0';

					sprintf(imgname_cy, "%s_%s.png", imgname_cy, effectsList[effectCount-1]);

					Log::writeLog("%s :: Writing effect [%s] in file [%s]\n", __FUNCTION__, effectsList[effectCount-1], imgname_cy);

					if(!cvSaveImage(imgname_cy,effectsList[effectCount-1].frame->data))
					{
						printf("Could not save: %s\n",imgname_cy);
					}
				}

				continue;

			case 'g':
				sprintf(effectName, "%s Gray", filename_cy);
				frameEffect = frameGray;
				break;

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

		Log::writeLog("%s :: Effect[%d] : [%s]", __FUNCTION__, effectCount, effectName);

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
		Log::writeLog("%s :: Destroy[%d] : [%s]", __FUNCTION__, i, effectsList[i].name);

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
		"MFIT -l M Apply low-pass filter with the kernel M",
		"MFIT -H M Apply high-pass filter with the kernel M",
		"MFIT -w    Write the last effect on a file",
		""
	};

	for (i=0 ; usage_cy[i][0] != '\0' ; i++)
	{
		printf("%s\n", usage_cy[i]);
	}
}
