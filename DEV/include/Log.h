class Log
{
	public:
		static int writefd(char* buf_cy);
		static int writetty(char* buf_cy);
		static int writeLog(const char *fmt, ...);
};
