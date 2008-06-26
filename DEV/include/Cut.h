/**
 *Classe respons�vel por indentificar
 *transi��es do tipo Corte.
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
	 *Processo para a defini��o da 
	 *limiar de corte para o processo
	 *de detec��o de transi��es do tipo
	 *corte.
	 */
	int defineTreshold();
	 
	int countPoints();
	 
};
