/**
 *Classe respons�vel por detectar
 *o dissolve.
 */
class Dissolve: public DetectTransitions
{
	private:
		Video *video;

	public:
		Dissolve();
		Dissolve(Video *vdo);
		
		void detectTransitions(Video* vdo, std::vector<Transition>*);

	/**
	 *Calcula a primeira derivada de
	 *uma curva.
	 */
		double calcFirstDerivative(double , double);
	 
	/**
	 *Calcula a curva de vari�ncia de um v�deo.
	 */
		void detectBorder(Frame*);

};
