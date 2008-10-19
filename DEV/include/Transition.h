#ifndef TRANSITION_INCLUDE
#define TRANSITION_INCLUDE
/**
 *Classe responsável por representar
 *uma transição.
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
		 *Guarda o tipo da transição
		 *0 - Corte
		 *1 - Fade-In
		 *2 - Fade-Out
		 *3 - Dissolve
		 */
		int type;

		/**
		 *Posição do frame (em hh:mm:ss)que 
		 *marca exatamente o ponto da 
		 *transição no vídeo.
		 */
		long posTransition;

		/**
		 *Posição do frame (em hh:mm:ss)que 
		 *marca exatamente o ponto da 
		 *transição no vídeo, que foi
		 *redefinido pelo editor.
		 */
		long posUserTransition;

		/**
		 *Identificador da transição na 
		 *timeline do vídeo.
		 */
		char* label;

	public:

		Transition();
		Transition(int, int, char*);

		bool selected;

		// Seta tipo da transição
		void setType(int type);
		
		// Retorna tipo da transição
		int getType();
	
		// Seta posição da transição (encontrada pelo sistema)
		void setPosTransition(int posTransition);
	
		// Retorna a posição da transição (encontrada pelo sistema)
		long getPosTransition();

		// Salva a posição da transição (modificada pelo usuário)
		void setPosUserTransition(int posUser);

		// Retorna a posição da transição (modificada pelo usuário)	
		long getPosUserTransition();

		// Define a label que irá identificar a transição na timeline
		void setLabel(char *label);

		// Retorna a label de definição da transição na timeline
		char* getLabel();

		bool operator <(const Transition& Rhs) const;

		long getPosCurrent(void) const;



};
#endif
