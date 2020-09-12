#include <iostream>
#include <fstream>
#include <sys/stat.h>

enum LogLevel
{
	LOG_INFO,
	LOG_ERR,
	LOG_REC,
	LOG_SIG,
};

class Tintin_reporter
{
	public:
		Tintin_reporter();
		bool IfExist(const std::string &s);
		void Log(LogLevel type, std::string s);
		std::string LogEvaluate(LogLevel type);
		std::string CurrentDateFormatted(void);


	private:
		std::ofstream	_file;
		std::string		_progname;
};
