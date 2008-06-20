#include "Frame.h"

/**
 *Classe respons�vel por criar
 *os dois tipos de ritmo visual.
 */
class VisualRhythm 
{

	public:
		/**
		 *Cria o Ritmo Visual por sub-amostragem.
		 */
		Frame createVR();

		/**
		 *Retorna o Ritmo Visual por 
		 *Histograma  do v�deo
		 */
		int createVRH(); 

		/**
		 *Calcula primeira derivada da curva do
		 *gr�fico do ritmo visual.
		 */
		float calcDerivatives();
};
