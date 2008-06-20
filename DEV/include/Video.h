#include "Time.h"
#include "Take.h"
#include "Frame.h"
#include "cv.h"
#include "highgui.h"

/**
 *Classe responsável por fazer
 *a manipulação de um vídeo.
 */
class Video
{
 
	/**
	 *Armazena o nome do vídeo
	 */
	private:
		char name;
	 
	/**
	 *Armazena o endereço do vídeo 
	 *carregado no sistema
	 */
		char path;
	 
	/**
	 *Armazena o vídeo carregado.
	 */
		CvCapture* data;
	 
	/**
	 *Armazena o tamanho do vídeo.
	 */
		Time length;
	 
	/**
	 *Armazena o codec do video.
	 *
	 */
		char codec;
	 
	/**
	 *Armazena o sistema de cores que
	 *se encontra o vídeo.
	 */
		int systemColor;
	 
	/**
	 *Armazena a taxa de frames
	 *por segundo do vídeo.
	 *
	 */
		float fps;
	 
	/**
	 *Nó da lista de tomadas.
	 */
		Take take;
	 
	public:
	/**
	 *Método responsável por obter
	 *o próximo frame em relação à
	 *posição atual do vídeo.
	 */
		Frame getNextFrame();
	 
	/**
	 *Método responsável por obter
	 *o frame anterior em relação à
	 *posição atual do vídeo.
	 */
		Frame getPreviousFrame();

	/**
	 *Método responsável em 
	 *posicionar o ponteiro do vídeo
	 *na posição desejada, a qual é
	 *parecida como parâmetro da 
	 *função.
	 */
		int seekFrame(int pos);
	 
	/**
	 *Função que abre o vídeo
	 *e atualiza as variáveis do objeto.
	 */
		CvCapture open(); 
	 
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
	Frame getCurrentFrame();
	 
	/**
	 *Obtêm a posição corrente do
	 *ponteiro do vídeo.
	 */
	int getCurrentPosition();
	 
	/**
	 *Processo que escreve no arquivo.
	 *É nele onde são aplicados
	 *realmente os efeitos provenientes
	 *da edição do vídeo.
	 *
	 *retorna:
	 *
	 * 1 - Sucesso.
	 *<0 - Erro.
	 */
	int write();
};
