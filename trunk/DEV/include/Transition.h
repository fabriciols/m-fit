/**
 *Classe respons�vel por representar
 *uma transi��o.
 */
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
		int posFrame;

		/**
		 *Posi��o do frame (em hh:mm:ss)que 
		 *marca exatamente o ponto da 
		 *transi��o no v�deo, que foi
		 *redefinido pelo editor.
		 */
		int posUserFrame;

		/**
		 *Identificador da transi��o na 
		 *timeline do v�deo.
		 */
		char* label;
};
