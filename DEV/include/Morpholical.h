#include "Frame.h"

/**
 *Classe que agrupa filtros de 
 *morfologia matem�tica.
 */
class Morpholical 
{
 
	private:
		Frame frame;
	 
	/**
	 *Aplica o efeito de Eros�o em uma
	 *imagem.
	 */
		Frame erode();
	 
	/**
	 *Aplica o efeito de dilata��o em
	 *uma imagem
	 *
	 */
		Frame dilate();
	 
};
