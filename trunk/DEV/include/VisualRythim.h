#include "Frame.h"

/**
 *Classe responsável por criar
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
		 *Histograma  do vídeo
		 */
		int createVRH(); 

		/**
		 *Calcula primeira derivada da curva do
		 *gráfico do ritmo visual.
		 */
		float calcDerivatives();
};
