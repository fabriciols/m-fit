#ifndef EFFECT_INCLUDE
#define EFFECT_INCLUDE
/**
 *Classe respons�vel por centralizar
 *os m�todos dos efeitos.
 */
class Effect
{
	private:
	/**
	 *Fun��o gen�rica para aplica��o
	 *de efeito. Ser� implementada
	 *por todas as classes filhas
	 *deste objeto.
	 */
	public:

		char name_cy[50];

		long frameStart;
		long frameEnd;

		virtual Frame* applyEffect(Frame *frame) = 0;

		long getFrameStart();
		long getFrameEnd();

		char *getName();

		// Define de TODOS os EFEITOS
		#define COLOR  0
		#define ERODE  1
		#define DILATE 2
};
#endif
