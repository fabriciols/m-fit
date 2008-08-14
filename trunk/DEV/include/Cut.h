/**
 *Classe responsável por indentificar
 *transições do tipo Corte.
 */
class Cut:DetectTransitions
{
 
	private:
		int treshould;
	 
	/**
	 *Cria o mapa de bordas.
	 */
	public:
		Frame* createBorderMap(Frame* visualRythim);
	 
		Transition* detectTransitions(Video* vdo, int type);

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
