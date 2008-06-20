#include "Histogram.h"
#include "cv.h"

/**
 *Classe que armazena informa��es sobre
 *um �nico frame.
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
	 	*Armazena a posi��o "f�sica" do
	 	*frame no v�deo.
	 	*/

		IplImage data;
		long posFrame;
	 
		/**
		*Fun��o que cria o histograma da
		*diagonal do frame.
		*/
		Histogram createHistogram();
	 
		/**
		*Retorna a diagonal do frame.
		*/
		Frame getDiagonal();
};
