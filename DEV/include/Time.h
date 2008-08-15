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
		
		Time();
		/**
		 *  Retorna o tempo (milisegundos) da ocorrencia da transi��o a
		 *  partir da posicao do frame.
		**/
		void pos2time(int position, double fps); 

		int getHour();
		int getMin();
		int getSec();
		int getMsec();

};
