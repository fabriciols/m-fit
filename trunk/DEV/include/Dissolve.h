/**
 *Classe responsável por detectar
 *o dissolve.
 */
class Dissolve: public DetectTransitions
{
	private:
		Video *video;

	public:
		void detectTransitions(Video* vdo, std::vector<Transition>*);

	/**
	 *Calcula a primeira derivada de
	 *uma curva.
	 */
		int calcFirstDerivative(double , double, double, double);
};
