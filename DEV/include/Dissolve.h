
/**
 *Classe responsável por detectar
 *o dissolve.
 */
class Dissolve: public DetectTransitions
{
 
	public:
		bool checkLuminance();
	 
	/**
	 *Calcula a segunda derivada de
	 *uma curva.
	 */
		float calcSecondDerivative();
	 
	/**
	 *Calcula a curva de variância de um vídeo.
	 */
		float calcVariance();
	 
	/**
	 *Calcula a razão entre a segunda
	 *derivada da variância com a 
	 *derivada do RVH.
	 */
		float calcRatioVarianceVRH();
	 
};
