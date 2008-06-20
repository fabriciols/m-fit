#include "Histogram.h"
#include "cv.h"

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
		int width;

		/**
	 	*Altura do frame
	 	*/
		int heigth;
	 
	public:

		/**
	 	*Armazena a posição "física" do
	 	*frame no vídeo.
	 	*/

		IplImage data;
		long posFrame;
	 
		/**
		*Função que cria o histograma da
		*diagonal do frame.
		*/
		Histogram createHistogram();
	 
		/**
		*Retorna a diagonal do frame.
		*/
		Frame getDiagonal();
};
