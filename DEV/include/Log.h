#ifndef LOG_INCLUDE
#define LOG_INCLUDE
class Log
{
	public:
		static int writefd(char* buf_cy);
		static int writetty(char* buf_cy);
		static int writeLog(const char *fmt, ...);
};
#endif
