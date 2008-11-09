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
		Time(double framePos, double fps);
		Time(int hour, int min, int sec, int msec);

		void pos2time(int position, double fps); 
		unsigned long getFramePos(double fps);


		// Passa o tempo de milisegundos para HH:MM:SS:MS
		void setTime(unsigned long msec);

		int getHour();
		int getMin();
		int getSec();
		int getMsec();

};
#endif
