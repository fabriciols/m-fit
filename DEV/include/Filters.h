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
		 *Ser� utilizado pelo processeo
		 *createBorderMap da classe Cut
		 *
		 */
		void Sobel(Frame* frame, int direction);
		/**
		 *Aplica um filtro passa-baixa na
		 *imagem.
		 *
		 *CvSmooth
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

};
