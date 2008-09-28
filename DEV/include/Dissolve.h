#ifndef DISSOLVE_INCLUDE
#define DISSOLVE_INCLUDE
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
#endif
