
/**
 *Classe respons�vel por detectar
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
	 *Calcula a curva de vari�ncia de um v�deo.
	 */
		float calcVariance();
	 
	/**
	 *Calcula a raz�o entre a segunda
	 *derivada da vari�ncia com a 
	 *derivada do RVH.
	 */
		float calcRatioVarianceVRH();
	 
};
