#ifndef EFFECT_INCLUDE
#define EFFECT_INCLUDE
/**
 *Classe respons�vel por centralizar
 *os m�todos dos efeitos.
 */
class Effect
{
	/**
	 *Fun��o gen�rica para aplica��o
	 *de efeito. Ser� implementada
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
