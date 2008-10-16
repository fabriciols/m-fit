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

		Frame* applyEffect(Frame *frame);

		long getFrameStart();
		long getFrameEnd();

		int type;

};
#endif
