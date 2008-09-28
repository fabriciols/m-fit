#ifndef TIME_INCLUDE
#define TIME_INCLUDE
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

		// Passa o tempo de milisegundos para HH:MM:SS:MS
		void setTime(unsigned long msec);

		int getHour();
		int getMin();
		int getSec();
		int getMsec();

};
#endif
