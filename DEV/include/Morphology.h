#include "Frame.h"

/**
 *Classe que agrupa filtros de 
 *morfologia matemática.
 */
class Morphology
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
		Frame* dilate(Frame* frame);
	 
};
