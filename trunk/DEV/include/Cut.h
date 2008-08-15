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

		Frame* createBorderMap(Frame* visualRythim);
	 
		void detectTransitions(Video *vdo, Transition *transitions);

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

};
