#include "Fade.h"

/**
 *Classe respons�vel por indentificar
 *transi��es do tipo Fade-out.
 */
class Fadeout: public Fade
{

	public:
		/**
		 *Encontra a M�nima derivada.
		 */
		float calcMinDerivative();

};
