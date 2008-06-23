#include "Histogram.h"
#include "cv.h"
#include "highgui.h"

#include <stdexcept>

/**
 *Classe que armazena informações sobre
 *um único frame.
 */
class Frame 
{
 
	/**
	 *Armazena uma imagem, no caso o 
	 *frame do tipo IplImage
	 *
	 */
	private:

		/**
	 	*Largura do frame
	 	*/
		int *width;

		/**
	 	*Altura do frame
	 	*/
		int *height;

	public:

		/*
		* Construtor que recebe um caminho.
		* Abre a imagem e preenche todos os atributos
		*/
		Frame(char *filename_cy);

		/*
		* Construtor que recebe uma imagem ja aberta
		* Preenche todos os atributos
		*/
		Frame(IplImage *img_src);

		/*
		* Construtor que recebe um frame ja instanciado
		* Faz a copia da instancia
		*/
		Frame(Frame *frame);

		// Retorna um FRAME em escala de cinza
		// referente a instância corrente
		Frame* convert2Gray();

		/**
	 	*Armazena a posição "física" do
	 	*frame no vídeo.
	 	*/

		IplImage* data;

		/**
		*Função que cria o histograma da
		*diagonal do frame.
		*/
		Histogram createHistogram();
	 
		/**
		*Retorna a diagonal do frame.
		*/
		Frame getDiagonal();
		
		// Gets

		int getWidth();
		int getHeight();
};
