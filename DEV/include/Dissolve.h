/**
 *Classe responsável por detectar
 *o dissolve.
 */
class Dissolve: public DetectTransitions
{
	public:
		void detectTransitions(Video* vdo, std::vector<Transition>*);

	/**
	 *Calcula a primeira derivada de
	 *uma curva.
	 */
		int calcFirstDerivative(double , double, double, double);
		double* calcVariance(Video*);
		double* calcDerivative(double *, int);
};
