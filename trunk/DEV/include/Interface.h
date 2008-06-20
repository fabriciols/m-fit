class Interface {
 
	/**
	 *Cria a timeline na tela.
	 */
	public:
		void showTimeLine();
	 
	/**
	 *Atualiza a timeline com informa��es
	 *dos v�deos/tomadas/transi��es.
	 */
		void updateTimeline();
	 
	/**
	 *Cria exibi��o da lista de tomadas
	 *do v�deo.
	 */
		void createListofTakes();
	 
	/**
	 *Cria container para exibi��o
	 *do v�deo.
	 */
		void showVideo();
	 
	/**
	 *M�todo chamado quando o usu�rio
	 *modifica a posi��o de uma transi��o
	 *j� detectada.
	 */
		void redefineTakes();
	 
};
