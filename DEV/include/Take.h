#include "Transition.h"

#ifndef TAKE_INCLUDE
#define TAKE_INCLUDE
/**
 *Container para tomadas de vídeo.
 */
class Take 
{

	private:

		/**
		*Armazena o tempo inicial da tomada
		*/
		int start;

		/**
		 *Armazena o tempo final da tomada
		 */
		int end;

		/**
		 *Ponteiro que aponta para 
		 *a transição referente à tomada 
		 *corrente.
		 */
		Transition transition;

	public:

		/**
		 *Ponteiro que aponta para a próxima
		 *tomada em relação à tomada corrente
		 */
		Take* next;

		/**
		 *Ponteiro que aponta para a tomada 
		 *anterior em relação à tomada 
		 *corrente
		 */
		Take* prev;

};
#endif
