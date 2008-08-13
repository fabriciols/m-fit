/**
 *Classe pai de todas as detecções
 *de transições.
 */
class DetectTransitions 
{
 
	/**
	 *Função principal na detecção
	 *de transições. Desencadeia
	 *a chamada da função com este
	 *mesmo nome nos objetos
	 *filhos.
	 */
	public:
		Transition* detectTransitions(Video* vdo, int type);
};
