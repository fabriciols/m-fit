/**
 *Classe responsável por detectar
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

		bool checkLuminance();
	 
	/**
	 *Calcula a segunda derivada de
	 *uma curva.
	 */
		double calcSecondDerivative(double *, int);
	 
	/**
	 *Calcula a curva de variância de um vídeo.
	 */
		double calcVariance(double *, int );
	 
	/**
	 *Calcula a razão entre a segunda
	 *derivada da variância com a 
	 *derivada do RVH.
	 */
		double calcRatioVarianceVRH(double, double);
	 
};
