/**
 *Classe que agrua informa��es
 *e manipula��o de tempo.
 */
class Time 
{
 
	private:
		int hour;
		int min;
		int sec;

		int msec;
	 
	public:
	/**
	 *Retorna o valor em POSI��O no 
	 *v�deo referente ao tempo armazenado
	 *por este objeto.
	 */
		int time2pos();

		void setTime(unsigned long msec);
};
