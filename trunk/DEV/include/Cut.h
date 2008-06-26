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
		Frame createBorderMap();
	 
	/**
	 *Processo para a definição da 
	 *limiar de corte para o processo
	 *de detecção de transições do tipo
	 *corte.
	 */
	int defineTreshold();
	 
	int countPoints();
	 
};
