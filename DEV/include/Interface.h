class Interface {
 
	/**
	 *Cria a timeline na tela.
	 */
	public:
		void showTimeLine();
	 
	/**
	 *Atualiza a timeline com informações
	 *dos vídeos/tomadas/transições.
	 */
		void updateTimeline();
	 
	/**
	 *Cria exibição da lista de tomadas
	 *do vídeo.
	 */
		void createListofTakes();
	 
	/**
	 *Cria container para exibição
	 *do vídeo.
	 */
		void showVideo();
	 
	/**
	 *Método chamado quando o usuário
	 *modifica a posição de uma transição
	 *já detectada.
	 */
		void redefineTakes();
	 
};
