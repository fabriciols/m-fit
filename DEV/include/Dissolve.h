/**
 *Classe responsável por detectar
 *o dissolve.
 */
class Dissolve: public DetectTransitions
{
	public:
		void detectTransitions(Video* vdo, std::vector<Transition>*);

		double* calculateVariance(Video*, double*);
		double* calculateDerivative(double *, int);
		int* validateDissolve(double*, double*, int, int);
};
