/**
 *Classe responsável por indentificar
 *transições do tipo Corte.
 */
class Cut: public DetectTransitions
{
 
	private:
		int treshould;
	 
	/**
	 *Cria o mapa de bordas.
	 */
	public:
		Frame* createBorderMap(Frame* visualRythim);
	 
		void detectTransitions(Video *vdo, Transition *transitions);

		int getTreshould();

		int setTreshould(int treshould);
	/**
	 *Processo para a definição da 
	 *limiar de corte para o processo
	 *de detecção de transições do tipo
	 *corte.
	 */
	int defineTreshould(int height);
	 
	int countPoints(Frame* borderMap, int treshould);

};
