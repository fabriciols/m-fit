/**
 *Classe respons�vel por detectar
 *o dissolve.
 */
class Dissolve: public DetectTransitions
{
	private:
		char *tmp;
		char tmp1[255];
		int itmp;
		Video *video;

	public:
		Dissolve();
		Dissolve(Video *vdo);
		
		void detectTransitions(Video* vdo, std::vector<Transition>* transitionList);

		bool checkLuminance();
	 
	/**
	 *Calcula a segunda derivada de
	 *uma curva.
	 */
		double* calcSecondDerivative(double *, int);
	 
	/**
	 *Calcula a curva de vari�ncia de um v�deo.
	 */
		double calcVariance(double *, int );
	 
	/**
	 *Calcula a raz�o entre a segunda
	 *derivada da vari�ncia com a 
	 *derivada do RVH.
	 */
		double calcRatioVarianceVRH(double, double);
	 
};