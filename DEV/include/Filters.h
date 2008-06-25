#include "Frame.h"

/**
 *Classe que agrupa filtros de
 *processamento de imagens digitais.
 */

class Filters 
{

	public:
		/**
		 *Aplica o Operador Sobel
		 *em uma imagem. 
		 *
		 *Será utilizado pelo processeo
		 *createBorderMap da classe Cut
		 *
		 */
		Frame* Sobel(Frame* frame, int direction);
		/**
		 *Aplica um filtro passa-baixa na
		 *imagem.
		 *
		 *CvSmooth
		 *
		 */
		Frame* lowPass(Frame* frame, int size);


		/**
		 *Aplica um filtro passa alta na 
		 *imagem.
		 */
		Frame* highPass(Frame* frame);

		/*
		* Aplica um threshold na imagem
		*/
		Frame* segment(Frame* frame, int threshold);

};
