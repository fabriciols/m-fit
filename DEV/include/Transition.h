/**
 *Classe responsável por representar
 *uma transição.
 */
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
		int posFrame;

		/**
		 *Posição do frame (em hh:mm:ss)que 
		 *marca exatamente o ponto da 
		 *transição no vídeo, que foi
		 *redefinido pelo editor.
		 */
		int posUserFrame;

		/**
		 *Identificador da transição na 
		 *timeline do vídeo.
		 */
		char* label;
};
