#ifndef FADEOUT_INCLUDE
#define FADEOUT_INCLUDE
/**
 *Classe responsável por indentificar
 *transições do tipo Fade-out.
 */
class Fadeout: public Fade
{

	public:
		/**
		 *Encontra a Mínima derivada.
		 */
		float calcMinDerivative();

};
#endif
