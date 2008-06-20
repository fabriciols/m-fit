#include "Fade.h"

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
