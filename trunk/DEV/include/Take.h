#include "Transition.h"

#ifndef TAKE_INCLUDE
#define TAKE_INCLUDE
/**
 *Container para tomadas de v�deo.
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
		 *a transi��o referente � tomada 
		 *corrente.
		 */
		Transition transition;

	public:

		/**
		 *Ponteiro que aponta para a pr�xima
		 *tomada em rela��o � tomada corrente
		 */
		Take* next;

		/**
		 *Ponteiro que aponta para a tomada 
		 *anterior em rela��o � tomada 
		 *corrente
		 */
		Take* prev;

};
#endif
