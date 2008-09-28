#ifndef MORPHOLOGY_INCLUDE
#define MORPHOLOGY_INCLUDE
/**
 *Classe que agrupa filtros de 
 *morfologia matemática.
 */
class Morphology
{
 
	public:
	 
	/**
	 *Aplica o efeito de Erosão em uma
	 *imagem.
	 */
		Frame* erode(Frame* frame);
	 
	/**
	 *Aplica o efeito de dilatação em
	 *uma imagem
	 *
	 */
		Frame* dilate(Frame* frame);
	 
};
#endif
