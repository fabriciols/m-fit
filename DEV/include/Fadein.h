#ifndef FADEIN_INCLUDE
#define FADEIN_INCLUDE
/**
 *Classe respons�vel por indentificar
 *transi��es do tipo Fade-in.
 */
class Fadein: public Fade
{
 
	/**
	 *Encontra a M�xima derivada.
	 */
	public:
		float calcMaxDerivative();
	 
};
#endif
