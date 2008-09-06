/**
 *Classe responsável por indentificar
 *transições do tipo Corte.
 */
class Cut: public DetectTransitions
{
 
	private:
		int threshold;
	 
	/**
	 *Cria o mapa de bordas.
	 */
	public:

		Cut(); // Construtor

		void createBorderMap(Frame* visualRythim);
	 
		void detectTransitions(Video* vdo, std::vector<Transition>* tansitionList);


		int getThreshold();

		int setThreshold(int threshold);
	/**
	 *Processo para a definição da 
	 *limiar de corte para o processo
	 *de detecção de transições do tipo
	 *corte.
	 */
	int defineThreshold(int height);
	 
	int* countPoints(Frame* borderMap, int threshold);

	// Valida se aquilo que foi detectado é realmente um corte.
	int validateCut(Frame* visual, int position);

};
