#ifndef TRANSITION_INCLUDE
#define TRANSITION_INCLUDE
/**
 *Classe respons�vel por representar
 *uma transi��o.
 */

#define TRANSITION_VIDEOSTART 0
#define TRANSITION_CUT        1
#define TRANSITION_FADEIN     2
#define TRANSITION_FADEOUT    3
#define TRANSITION_DISSOLVE   4

class Transition
{

	private:

		/**
		 *Guarda o tipo da transi��o
		 *0 - Corte
		 *1 - Fade-In
		 *2 - Fade-Out
		 *3 - Dissolve
		 */
		int type;

		/**
		 *Posi��o do frame (em hh:mm:ss)que 
		 *marca exatamente o ponto da 
		 *transi��o no v�deo.
		 */
		long posTransition;

		/**
		 *Posi��o do frame (em hh:mm:ss)que 
		 *marca exatamente o ponto da 
		 *transi��o no v�deo, que foi
		 *redefinido pelo editor.
		 */
		long posUserTransition;

		/**
		 *Identificador da transi��o na 
		 *timeline do v�deo.
		 */
		char* label;

	public:

		Transition();
		Transition(int, int, char*);

		bool selected;

		// Seta tipo da transi��o
		void setType(int type);
		
		// Retorna tipo da transi��o
		int getType();
	
		// Seta posi��o da transi��o (encontrada pelo sistema)
		void setPosTransition(int posTransition);
	
		// Retorna a posi��o da transi��o (encontrada pelo sistema)
		long getPosTransition();

		// Salva a posi��o da transi��o (modificada pelo usu�rio)
		void setPosUserTransition(int posUser);

		// Retorna a posi��o da transi��o (modificada pelo usu�rio)	
		long getPosUserTransition();

		// Define a label que ir� identificar a transi��o na timeline
		void setLabel(char *label);

		// Retorna a label de defini��o da transi��o na timeline
		char* getLabel();

		bool operator <(const Transition& Rhs) const;

		long getPosCurrent(void) const;



};
#endif
