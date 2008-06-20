/**
 *Classe que agrua informações
 *e manipulação de tempo.
 */
class Time 
{
 
	private:
		int hour;
		int min;
		int sec;
	 
	public:
	/**
	 *Retorna o valor em POSIÇÃO no 
	 *vídeo referente ao tempo armazenado
	 *por este objeto.
	 */
		int time2pos();
};
