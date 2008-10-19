#ifndef EFFECT_INCLUDE
#define EFFECT_INCLUDE
/**
 *Classe responsável por centralizar
 *os métodos dos efeitos.
 */
class Effect
{
	/**
	 *Função genérica para aplicação
	 *de efeito. Será implementada
	 *por todas as classes filhas
	 *deste objeto.
	 */
	public:

		long frameStart;
		long frameEnd;

		virtual Frame* applyEffect(Frame *frame) = 0;

		long getFrameStart();
		long getFrameEnd();

		// Define de TODOS os EFEITOS
		#define COLOR  0
		#define ERODE  1
		#define DILATE 2
};
#endif
