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
		int msec;
	 
	public:
		
		Time();
		/**
		 *  Retorna o tempo (milisegundos) da ocorrencia da transição a
		 *  partir da posicao do frame.
		**/
		void pos2time(int position, double fps); 

		int getHour();
		int getMin();
		int getSec();
		int getMsec();

};
