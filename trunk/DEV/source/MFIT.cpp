#include "cv.h"
#include "highgui.h"

#include "../include/Log.h"
#include "../include/Time.h"

#include "../include/Histogram.h"
#include "../include/Frame.h"
#include "../include/Video.h"

#include "../include/Effect.h"
#include "../include/Color.h"

#include "../include/Filters.h"
#include "../include/Morphology.h"

#include "../include/VisualRythim.h"

#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#define EFFECT_NAME_SIZE 30
#define MAX_OPTIONS 30

void usage();

/************************************************************************
* Fun��o main.
*************************************************************************
* param (E): argc e argv.
*************************************************************************
* return : nenhum.
*************************************************************************
* Hist�rico:
* 29/07/08 - Thiago Mizutani
* Retirando alguns parametros desnecess�rios.
* Algum dia algu�m criou. Coloquem historico a partir daqui.
************************************************************************/

int main(int argc, char* argv[])
{

	Frame *frame; // Frame original

	Frame *frameGray;	// Frame original em escala de cinza

	Frame *frameEffect; 	// Frame original com o efeito aplicado

	Color *color; 	// Objeto de efeitos de cor

	Video *vdo;

	CvCapture* video = 0; 

	char filename_cy[100];	// Nome do arquivo

	char extension_cy[4];

	char *effectName;	// Nome do efeito aplicado

	int i, aux_i, effectCount = 0;

	int detectNow = 0;

	enum inputType { IMAGE, VIDEO }; // Tipo da entrada

	inputType input;

	/** 
	 * Estrutura para armazenar a lista de efeitos aplicados
	 *  possibilitando assim aplicar efeitos sobre efeitos,
	 *  e aumentando a mem�ria usada :)
	 **/
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

		// Pega somente a extensao do arquivo
		strcpy(extension_cy, &filename_cy[strlen(filename_cy)-3]);

		Log::writeLog("%s :: file extension[%s]", __FUNCTION__, extension_cy);

		// Verifica se o arquivo carregado � um v�deo ou imagem.
		if (!strcmp(extension_cy,"AVI") || !strcmp(extension_cy,"avi"))
		{
			input = VIDEO;
		}
		else
		{
			input = IMAGE;
		}
	}
	else
	{
		strcpy(filename_cy, "teste.jpg");
	}

	// Tenta abrir o arquivo
	try
	{

		if (input == VIDEO)
		{
			Log::writeLog("%s :: new Video filename[%s]", __FUNCTION__, filename_cy);

			vdo = new Video(filename_cy); // Instancia um objeto da classe video

			Log::writeLog("%s :: new Video Loaded [%x]", __FUNCTION__, vdo);
		}
		else
		{
			Log::writeLog("%s :: new Frame filename[%s]", __FUNCTION__, filename_cy);

			frame = new Frame(filename_cy); // Instancia um objeto da classe video

			Log::writeLog("%s :: new Frame Loaded [%x]", __FUNCTION__, frame);
		}
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

	// Todos os tratamentos usamos img em cinza, ent�o vamos 
	// transformar nosso frame em cinza.

	if (input == IMAGE) // � imagem (isso vai sumir depois... s� vai servir de exemplo.)
	{
		color = new Color();

		// converte a imagem para tons de cinza.
		frameGray = color->convert2Gray(frame);

		// Fa�o a varredura nos parametros passados pela linha de comando
		for (i = 2 ; i < argc ; i++)
		{

			if (argv[i][0] != '-')
				continue;

			Log::writeLog("%s :: param[%d]", __FUNCTION__, argc);
			Log::writeLog("%s :: param: argv[%d] = [%s]", __FUNCTION__, i, argv[i]);

			// Cria uma nova string para colocar na lista de efeitos
			effectName = new char[ sizeof(char)*EFFECT_NAME_SIZE ];

			// Zera a vari�vel auxiliar
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
				case 'f':
					{
						strcpy(effectName, "Diagonal");

						Frame *frame2;

						frame2 = frame->getDiagonal();

						frameEffect = frame2;

						break;
					}
				case 'z':
					{
						Frame *a;
						Frame b;

						strcpy(effectName, "operator=");

						a = new Frame(frame);

						b = *a;

						Log::writeLog("%s :: b[%x]", __FUNCTION__, &b);

						frameEffect = &b;

						frameEffect->write(effectName);

						break;
					}
				case 'e':
					{

						Morphology *morph = new Morphology();

						strcpy(effectName, "Erode");

						frameEffect = morph->erode(frameGray);

						break;

					}
				case 'c':
					{

						Frame *frame2;
						Frame *frame3;

						strcpy(effectName, "Concatenate");

						frame2 = new Frame(argv[i+1]);
						frame3 = new Frame(frame);

						Log::writeLog("%s :: frame3[%x] + frame2[%x]", __FUNCTION__, frame3, frame2);
						Log::writeLog("%s :: frame3->data[%x]", __FUNCTION__, frame3->data);

						for (i=0 ; i <= 10 ; i++)
						{
							*frame3 += *frame2;
						}

						Log::writeLog("%s :: frame3[%x] + frame2[%x]", __FUNCTION__, frame3, frame2);
						Log::writeLog("%s :: frame3->data[%x]", __FUNCTION__, frame3->data);

						frameEffect = frame3;

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

						//Se for passado algum argumento como valor para tamanho da m�scara
						//ser� = tamanho passado, sen�o assume por default o valor 5.

						if ( i < (argc-1) ) // O ultimo argumento � argc-1
						{
							if (argv[i+1][0] <= '9' && argv[i+1][0] >= '0')
							{
								Log::writeLog("%s :: LowPass param[%s]", __FUNCTION__, argv[i+1]);

								aux_i = atoi(argv[++i]); //Passo o valor do proximo parametro passado
							}
						}

						// 5 � o padr�o
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
						// Se o usu�rio n�o escolher o tipo de matriz a ser usada o sistema adota
						// uma como padr�o.

						if (('0' <= atoi(argv[i+1]) <= '9') && (i <= argc))	
						{
							aux_i = atoi(argv[++i]); // Incrementa i para que a proxima analise do for n�o pegue o mesmo parametro
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
						
						Histogram *hist;
						Frame *frameAux;

						// Se ja foi aplicado algum tipo de efeito, realizamo o histograma do ultimo efeito aplicado
						if (effectCount >= 1)
						{

							frameAux = effectsList[effectCount-1].frame;

							sprintf(effectName, "Histogram of %s", effectsList[effectCount-1].name);

							Log::writeLog("%s :: Histogram from effect[%d] [%s]\n",
									__FUNCTION__, effectCount-1, effectsList[effectCount-1].name);
						}
						else // Sen�o, aplicamos na imagem original mesmo
						{
							frameAux = frameGray;
							sprintf(effectName, "Histogram of %s", filename_cy);
						}

					//	Histogram *hist = new Histogram(frameAux->data);
						hist = frameAux->createHistogram();

						Log::writeLog("%s :: min[%d]: [%.0f], max[%d]: [%.0f]",
								__FUNCTION__, hist->getMinLuminance(), hist->getMin(), hist->getMaxLuminance(), hist->getMax());

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


			Log::writeLog("%s :: Effect[%d] : [%s] = [%x]", __FUNCTION__, effectCount, effectName, frameEffect);

			// Salva os dados na estrutura, para poder aplicar efeitos na imagem anterior
			effectsList[effectCount].name  = effectName;
			effectsList[effectCount].frame = frameEffect;

			// Imprime na tela
			cvNamedWindow(effectsList[effectCount].name, 1);
			cvShowImage(effectsList[effectCount].name, effectsList[effectCount].frame->data);

			effectCount++;

		}

		cvWaitKey(0);

		// Limpa a mem�ria
		for (i=0 ; i < effectCount ; i++)
		{
			Log::writeLog("%s :: Destroy[%d] : [%s]", __FUNCTION__, i, effectsList[i].name);

			cvDestroyWindow(effectsList[i].name);
			delete effectsList[i].name;
			delete effectsList[i].frame;
		}
	}
	else if (input == VIDEO) // � v�deo
	{
		cvNamedWindow(vdo->getName(), 1);

		for (i = 2 ; i < argc ; i++)
		{

			if (argv[i][0] != '-')
				continue;

			Log::writeLog("%s :: param[%d]", __FUNCTION__, argc);
			Log::writeLog("%s :: param: argv[%d] = [%s]", __FUNCTION__, i, argv[i]);

			// Zera a vari�vel auxiliar
			aux_i = 0;

			switch (argv[i][1])
			{
				case 'h':
				{
					char imgname_cy[100];

					// Monta o ritmo visual por histograma
					VisualRythim *vrh;
					Frame *frameVRH;

					int vdoSize = 0;
					double *visual;

					Log::writeLog("%s :: createVRH", __FUNCTION__);

					visual = vrh->createVRH(vdo);

					vdoSize = cvRound(vdo->getFramesTotal());

					Log::writeLog("%s :: create VRH frame", __FUNCTION__);

					//frameVRH = new Frame((double*)visual, vdoSize, 256);
					frameVRH = new Frame(visual, vdoSize, 256);

					sprintf(imgname_cy, "vrh_%s.jpg", vdo->getName());

					frameVRH->write(imgname_cy);

					cvShowImage(vdo->getName(), frameVRH->data);

					cvWaitKey(0);

					break;
				}
				case 'r':
				{
					// Monta o ritmo-visual
					char imgname_cy[100];

					VisualRythim *vr;
					Frame *vr_frame;

					Log::writeLog("%s :: VisualRythim video[%s]", __FUNCTION__, vdo->getName());

					vr = new VisualRythim();

					Log::writeLog("%s :: createVR[%x]", __FUNCTION__, vdo);

					vr_frame = vr->createVR(vdo);

					cvShowImage(vdo->getName(), vr_frame->data);

					sprintf(imgname_cy, "vr_%s.jpg", vdo->getName());

					vr_frame->write(imgname_cy);

					cvWaitKey(0);

					delete vr_frame;
					delete vdo;

					break;
				}
				case 'n':
					{
						// Navigate
						char c;
						Frame *frameVideo;

						frameVideo = vdo->getCurrentFrame();

						do
						{
							cvShowImage(vdo->getName(), frameVideo->data);

							c = cvWaitKey(0);

							delete frameVideo;

							Log::writeLog("%s :: pressed[%d][%c]", __FUNCTION__, c, c);

							// 44 = "<"
							if (c == 44)
							{
								frameVideo = vdo->getPreviousFrame();
							} // 46 = ">"
							else if (c == 46)
							{
								frameVideo = vdo->getNextFrame();
							} // -1 = Fechar a janela 27 = ESC
							else if (c == -1 || c == 27)
							{
								break;
							}
							else
							{
								continue;
							}
						}
						while(true);

						delete frameVideo;

						break;
					}
			}

			break;
		}

		cvDestroyWindow(vdo->getName());

	}

	cvWaitKey(0);
}

void usage()
{
	int i;
	char *usage_cy[] = {
		"Use: MFIT.exe img/video -[t|d|e|s|h|p|g|l|H|w|n|z|f]  - Apply filter in img/video\n",
		"MFIT image -t T Segmentation of the image with the threshold T",
		"MFIT image -d   Dilate the image",
		"MFIT image -e   Erode the image",
		"MFIT image -sh  Apply Sobel (Horizontal) filter in the image",
		"MFIT image -sv  Apply Sobel (Vertial) filter in the image",
		"MFIT image -s   Apply Sobel (Both) filter in the image",
		"MFIT image -h   Print the histogram of the last img",
		"MFIT image -p   Print the img_src",
		"MFIT image -g   Print the img_src in gray",
		"MFIT image -l M Apply low-pass filter with the kernel M",
		"MFIT image -H M Apply high-pass filter with the kernel M",
		"MFIT image -w   Write the last effect on a file",
		"MFIT image -z   Simulate a '=' operation with frame",
		"MFIT image -f   Get a image diagonal",
		"MFIT video -n   Navigate using '<' and '>' in a video file",
		"MFIT video -v   Generate a Visual Rythm",
		"MFIT video -h   Generate a Visual Rythm Histogram",
		""
	};

	for (i=0 ; usage_cy[i][0] != '\0' ; i++)
	{
		printf("%s\n", usage_cy[i]);
	}
}
