/************************************************************************
* Classe que armazena infoma��es sobre um �nico frame.
*************************************************************************
* Hist�rico:
* 30/06/08 - Thiago Mizutani
* Retirada do m�todo convert2Gray(). Agora este faz parte da classe Color
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/

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

		/*
		* Construtor que recebe uma matriz(histograma) e seu tamanho
		* e faz o seu desenho
		*/
		Frame(double matrix[], int len_i, float max_f);

		/**
	 	*Armazena a posi��o "f�sica" do
	 	*frame no v�deo.
	 	*/

		IplImage* data;

		/**
		*Fun��o que cria o histograma da
		*diagonal do frame.
		*/
		//Histogram createHistogram();
	 
		/**
		*Retorna a diagonal do frame.
		*/
		Frame getDiagonal();
		
		// Gets

		int getWidth();
		int getHeight();
};
