/**
 *Classe que agrupa filtros de
 *processamento de imagens digitais.
 */

#define SOBEL_VERTICAL	  0
#define SOBEL_HORIZONTAL  1
#define SOBEL_COMPLETE	  2

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
		void Sobel(Frame* frame, int direction);
		/**
		 *Aplica um filtro passa-baixa na
		 *imagem.
		 *
		 */
		void lowPass(Frame* frame, int size);

		/**
		 *Aplica um filtro passa alta na 
		 *imagem.
		 */
		void highPass(Frame* frame, int typeMask);

		/*
		* Aplica um threshold na imagem
		*/
		void segment(Frame* frame, int threshold);

		void Canny(Frame* frame, double thresholdMin, double thresholdMax, int size);

};
