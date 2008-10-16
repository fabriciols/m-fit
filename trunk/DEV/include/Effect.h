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
	protected:
		long frameStart;
		long frameEnd;

	public:

		virtual Frame* applyEffect(Frame *frame) = 0;

		long getFrameStart();
		long getFrameEnd();

};
#endif
