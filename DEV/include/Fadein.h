#include "Fade.h"

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
