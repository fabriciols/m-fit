#ifndef MORPHOLOGY_INCLUDE
#define MORPHOLOGY_INCLUDE
/**
 *Classe que agrupa filtros de 
 *morfologia matem�tica.
 */
class Morphology
{
 
	public:
	 
	/**
	 *Aplica o efeito de Eros�o em uma
	 *imagem.
	 */
		Frame* erode(Frame* frame);
	 
	/**
	 *Aplica o efeito de dilata��o em
	 *uma imagem
	 *
	 */
		Frame* dilate(Frame* frame);
	 
};
#endif
