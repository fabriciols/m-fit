
/**
 *Classe respons�vel por centralizar
 *os m�todos dos efeitos.
 */
class Effect
{
	private:
		Frame frame;
	 
	/**
	 *Fun��o gen�rica para aplica��o
	 *de efeito. Ser� implementada
	 *por todas as classes filhas
	 *deste objeto.
	 */
	public:

		Effect* Effect();

		int applyEffect();
};
