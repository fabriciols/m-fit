#ifndef DETECTTRANSITIONS_INCLUDE
#define DETECTTRANSITIONS_INCLUDE
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
		void detectTransitions(Video* vdo, std::vector<Transition>* tansitionList);

		// Verifica se já existem transições detectadas na posição da nova transição.
		bool validateTransition(long position, std::vector<Transition>* transitionList);

};
#endif
