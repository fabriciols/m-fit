#ifndef FADEIN_INCLUDE
#define FADEIN_INCLUDE
/**
 *Classe responsável por indentificar
 *transições do tipo Fade-in.
 */
class Fadein: public Fade
{
 
	/**
	 *Encontra a Máxima derivada.
	 */
	public:
		float calcMaxDerivative();
	 
};
#endif
