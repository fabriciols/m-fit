/************************************************************************
* Classe respons�vel em aplicar efeitos de cor.
*************************************************************************
* Hist�rico:
* 30/06/08 - Thiago Mizutani
* Inclus�o do m�todo convert2Gray(). Foi transferido da classe Frame para
* esta.
* 27/06/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/

class Color: public Effect
{
	public:

	Effect* Color();

	// Converte um frame de RGB para escala de cinza.
	Frame* convert2Gray(Frame*frame);
 
}
