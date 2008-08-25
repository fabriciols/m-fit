/************************************************************************
* Classe que armazena infomações sobre um único frame.
*************************************************************************
* Histórico:
* 30/06/08 - Thiago Mizutani
* Retirada do método convert2Gray(). Agora este faz parte da classe Color
* 27/06/08 - Fabricio Lopes de Souza
* Criação.
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
		Frame(double *matrix, int len_i, float max_f);

		/**
	 	*Armazena a posição "física" do
	 	*frame no vídeo.
	 	*/

		IplImage* data;

		/**
		*Função que cria o histograma da
		*diagonal do frame.
		*/
		Histogram* createHistogram();
	 
		/**
		*Retorna a diagonal do frame.
		*/
		Frame * getDiagonal();
		
		/**
		 *	Binariza a imagem de entrada baseando-se em um limiar.
		 *	O limiar será = a 1/4 da maior luminancia presente na imagem.
		 *	Este critério é baseado em um artigo
		 * **/
		void binarizeImage(Frame* frame, int treshold);	

		double mediaBin(Frame* frame);

		// Gets

		// Retorna largura do frame
		int getWidth();

		// Retorna altura do frame
		int getHeight();

		// Seta o valor de luminancia determinado pixel.
		void setPixel(int x, int y, int lum);

		// Retorna o valor da luminancia de um pixel.
		int getPixel(int x, int y);

		Frame& operator+=(Frame& frame);
		Frame&  operator=(Frame& frame);

		~Frame(); // Destrutor????
		Frame();  // Construtor: inicializa as variaveis tds com 0.

		// FABRICIO COMENTA ESSAS FUNÇÕES!!!!
		void setImage(IplImage *imgNew);
		void initAttr();
		void write(char *filename_cy);
		static void imgCopy(IplImage *imgSrc, IplImage *imgDst);
		static IplImage* imgAlloc(CvSize size, int depth, int channels);
		static IplImage* imgAlloc(Frame *frame);
		static void imgDealloc(IplImage* img);

};

