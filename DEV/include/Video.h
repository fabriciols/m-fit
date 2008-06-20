#include "Time.h"
#include "Take.h"
#include "Frame.h"
#include "cv.h"
#include "highgui.h"

/**
 *Classe respons�vel por fazer
 *a manipula��o de um v�deo.
 */
class Video
{
 
	/**
	 *Armazena o nome do v�deo
	 */
	private:
		char name;
	 
	/**
	 *Armazena o endere�o do v�deo 
	 *carregado no sistema
	 */
		char path;
	 
	/**
	 *Armazena o v�deo carregado.
	 */
		CvCapture* data;
	 
	/**
	 *Armazena o tamanho do v�deo.
	 */
		Time length;
	 
	/**
	 *Armazena o codec do video.
	 *
	 */
		char codec;
	 
	/**
	 *Armazena o sistema de cores que
	 *se encontra o v�deo.
	 */
		int systemColor;
	 
	/**
	 *Armazena a taxa de frames
	 *por segundo do v�deo.
	 *
	 */
		float fps;
	 
	/**
	 *N� da lista de tomadas.
	 */
		Take take;
	 
	public:
	/**
	 *M�todo respons�vel por obter
	 *o pr�ximo frame em rela��o �
	 *posi��o atual do v�deo.
	 */
		Frame getNextFrame();
	 
	/**
	 *M�todo respons�vel por obter
	 *o frame anterior em rela��o �
	 *posi��o atual do v�deo.
	 */
		Frame getPreviousFrame();

	/**
	 *M�todo respons�vel em 
	 *posicionar o ponteiro do v�deo
	 *na posi��o desejada, a qual �
	 *parecida como par�metro da 
	 *fun��o.
	 */
		int seekFrame(int pos);
	 
	/**
	 *Fun��o que abre o v�deo
	 *e atualiza as vari�veis do objeto.
	 */
		CvCapture open(); 
	 
	/**
	 *Fun��o que fecha o v�deo.
	 *1 - sucesso.
	 *0 - erro.
	 *
	 */
		int close();
	 
	/**
	 *Obt�m o frame atual.
	 */
	Frame getCurrentFrame();
	 
	/**
	 *Obt�m a posi��o corrente do
	 *ponteiro do v�deo.
	 */
	int getCurrentPosition();
	 
	/**
	 *Processo que escreve no arquivo.
	 *� nele onde s�o aplicados
	 *realmente os efeitos provenientes
	 *da edi��o do v�deo.
	 *
	 *retorna:
	 *
	 * 1 - Sucesso.
	 *<0 - Erro.
	 */
	int write();
};
