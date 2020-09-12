#include "Tintin_reporter.h"


Tintin_reporter::Tintin_reporter(void)
{
	if (IfExist("/var/log/matt_daemon"))
	{
		_file.open("/var/log/matt_daemon/matt_daemon.log", std::ios::out | std::ios::app);
	}
	else
	{
		mkdir("/var/log/matt_daemon", 0600);
		_file.open("/var/log/matt_daemon/matt_daemon.log", std::ios::out | std::ios::app);
	}
	_progname = "Matt_daemon";
    Log(LOG_INFO, "Started.");
}

bool Tintin_reporter::IfExist(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

std::string Tintin_reporter::CurrentDateFormatted(void)
{
	time_t	now = time(0);
	char 	format[30];

	tm *ltm = localtime(&now);
	strftime(format, 30, "[%m/%d/%Y-%H:%M:%S]", ltm);

	return (std::string(format));
}

void Tintin_reporter::Log(LogLevel l, std::string s)
{
	if (!_file.is_open())
		_file.open("./var/log/matt_daemon/matt_daemon.log", std::ios::out | std::ios::app);
	_file << CurrentDateFormatted();
	_file << LogEvaluate(l);
	_file << "- " << _progname << ": ";
	_file << s << std::endl;
}

std::string Tintin_reporter::LogEvaluate(LogLevel type)
{
	switch (type)
	{
		case LOG_INFO:
			return (" [ INFO ] ");
		case LOG_ERR:
			return (" [ ERROR ] ");
		case LOG_REC:
			return (" [ LOG ] ");
		case LOG_SIG:
			return (" [ SIG ] ");
		break ;
	}
	return ("[ UNKNOWN ] ");
}
