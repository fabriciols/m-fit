// Classe responsável por fazer a manipulação de um vídeo.
class Video
{

	/**
	 *Armazena o nome do vídeo
	 */
	private:
		char name[300+1];

		/**
		 *Armazena o endereço do vídeo 
		 *carregado no sistema
		 */
		char path[300+1];


		/**
		 *Armazena o tamanho do vídeo.
		 */
		//		Time length;

		/**
		 *Armazena o codec do video.
		 *
		 */
		double codec;

		double framesHeight;

		double framesWidth;

		double framePos;

		double framesTotal;

		Time timePos;

		/**
		 *Armazena o sistema de cores que
		 *se encontra o vídeo.
		 */
		double systemColor;

		/**
		 *Armazena a taxa de frames
		 *por segundo do vídeo.
		 *
		 */
		double fps;

		/**
		 *Nó da lista de tomadas.
		 */
		//	Take take;

		/**
		 *Função que abre o vídeo
		 *e atualiza as variáveis do objeto.
		 */
		CvCapture* open(char *vdoSrc); 

	public:

		char* getName();
		char* getPath();
		/**
		 *Armazena o vídeo carregado.
		 */
		CvCapture* data;

		Video(char *filename_cy);

		/**
		 *Método responsável por obter
		 *o próximo frame em relação à
		 *posição atual do vídeo.
		 */
		Frame* getNextFrame();

		/**
		 *Método responsável por obter
		 *o frame anterior em relação à
		 *posição atual do vídeo.
		 */
		Frame* getPreviousFrame();

		/**
		 * Método responsável em posicionar o ponteiro do vídeo na posição desejada,
		 * a qual é parecida como parâmetro da função.
		 **/
		int seekFrame(unsigned long posFrame);


		/**
		 *Função que fecha o vídeo.
		 *1 - sucesso.
		 *0 - erro.
		 *
		 */
		int close();

		/**
		 *Obtém o frame atual.
		 */
		Frame* getCurrentFrame();

		double getFramesTotal();

		// Obtêm a posição corrente do ponteiro do vídeo.
		double getCurrentPosition();

		/**
		 * Processo que escreve no arquivo. É nele onde são aplicados
		 * realmente os efeitos provenientes da edição do vídeo.
		 * retorna:
		 * 1 - Sucesso.
		 * 0 - Erro.
		 **/
		int write();

		double getFramesWidth();
		double getFramesHeight();
		void updatePos();
		//	double getFramePos();
};
