/**
 *Classe responsável por 
 *identificar todos os tipos de
 *fade.
 */
class Fade: public DetectTransitions
{
	public:
		double* calcDerivative(double *array, int size_i);
		void detectTransitions(Video* vdo, std::vector<Transition>* transitionList);

};
