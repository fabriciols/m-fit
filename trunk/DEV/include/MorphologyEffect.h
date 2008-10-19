#ifndef MORPEFFECT_INCLUDE
#define MORPEFFECT_INCLUDE
/************************************************************************
* Classe responsável em aplicar efeitos de erosao.
*************************************************************************
* 19/10/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
class Erode: public Effect
{
	public:
		Frame* applyEffect(Frame *frame);
};

/************************************************************************
* Classe responsável em aplicar efeitos de dilatacao.
*************************************************************************
* 19/10/08 - Fabricio Lopes de Souza
* Criação.
************************************************************************/
class Dilate: public Effect
{
	public:
		Frame* applyEffect(Frame *frame);
};
#endif
