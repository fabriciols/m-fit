/**
 *Classe respons�vel por representar
 *uma transi��o.
 */

#define CUT 		1
#define FADEIN 	2
#define FADEOUT 	3
#define DISSOLVE 	4

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
		int posTransition;

		/**
		 *Posi��o do frame (em hh:mm:ss)que 
		 *marca exatamente o ponto da 
		 *transi��o no v�deo, que foi
		 *redefinido pelo editor.
		 */
		int posUserTransition;

		/**
		 *Identificador da transi��o na 
		 *timeline do v�deo.
		 */
		char* label;

	public:

		Transition();

		// Pr�xima transi��o detectada na lista 
		Transition* next;

		// Transi��o detectada anterior da lista
		Transition* previous;

		// Seta tipo da transi��o
		void setType(int type);
		
		// Retorna tipo da transi��o
		int getType();
	
		// Seta posi��o da transi��o (encontrada pelo sistema)
		void setPosTransition(int posTransition);
	
		// Retorna a posi��o da transi��o (encontrada pelo sistema)
		int getPosTransition();

		// Salva a posi��o da transi��o (modificada pelo usu�rio)
		void setPosUserTransition(int posUser);

		// Retorna a posi��o da transi��o (modificada pelo usu�rio)	
		int getPosUserTransition();

		// Define a label que ir� identificar a transi��o na timeline
		void setLabel(char *label);

		// Retorna a label de defini��o da transi��o na timeline
		char* getLabel();

};