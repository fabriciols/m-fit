
/**
 *Classe responsável por centralizar
 *os métodos dos efeitos.
 */
class Effect
{
	private:
		Frame frame;
	 
	/**
	 *Função genérica para aplicação
	 *de efeito. Será implementada
	 *por todas as classes filhas
	 *deste objeto.
	 */
	public:

		Effect* Effect();

		int applyEffect();
};
