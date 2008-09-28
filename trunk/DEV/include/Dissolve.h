#ifndef DISSOLVE_INCLUDE
#define DISSOLVE_INCLUDE
/**
 *Classe responsável por detectar
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
#endif
