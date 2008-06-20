#include "Frame.h"

/**
 *Classe que agrupa filtros de 
 *morfologia matemática.
 */
class Morpholical 
{
 
	private:
		Frame frame;
	 
	/**
	 *Aplica o efeito de Erosão em uma
	 *imagem.
	 */
		Frame erode();
	 
	/**
	 *Aplica o efeito de dilatação em
	 *uma imagem
	 *
	 */
		Frame dilate();
	 
};
