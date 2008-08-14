/**
 *Classe respons�vel por indentificar
 *transi��es do tipo Corte.
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
	 *Processo para a defini��o da 
	 *limiar de corte para o processo
	 *de detec��o de transi��es do tipo
	 *corte.
	 */
	int defineTreshould(int height);
	 
	int countPoints(Frame* borderMap, int treshould);

};
