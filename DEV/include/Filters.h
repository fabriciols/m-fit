#include "Frame.h"

/**
 *Classe que agrupa filtros de
 *processamento de imagens digitais.
 */

class Filters 
{

	private:
		Frame frame;

		/**
		 *Aplica o Operador Sobel
		 *em uma imagem. 
		 *
		 *Será utilizado pelo processeo
		 *createBorderMap da classe Cut
		 *
		 */
	public:
		Frame Sobel(); 
		/**
		 *Aplica um filtro passa-baixa na
		 *imagem.
		 *
		 *CvSmooth
		 *
		 */
		Frame lowPass();

		/**
		 *Aplica um filtro passa alta na 
		 *imagem.
		 */
		Frame highPass();

};
