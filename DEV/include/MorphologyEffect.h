#ifndef MORPEFFECT_INCLUDE
#define MORPEFFECT_INCLUDE
/************************************************************************
* Classe respons�vel em aplicar efeitos de erosao.
*************************************************************************
* 19/10/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
class Erode: public Effect
{
	public:
		Frame* applyEffect(Frame *frame);
};

/************************************************************************
* Classe respons�vel em aplicar efeitos de dilatacao.
*************************************************************************
* 19/10/08 - Fabricio Lopes de Souza
* Cria��o.
************************************************************************/
class Dilate: public Effect
{
	public:
		Frame* applyEffect(Frame *frame);
};
#endif
