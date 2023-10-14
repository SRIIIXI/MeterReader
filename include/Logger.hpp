#ifndef EDMI_LOGGER
#define EDMI_LOGGER

#include <map>
#include <string>

typedef enum LogLevel
{
	LogInfo = 0,
	LogError = 1,
	LogWarning = 2,
	LogCritical = 3
}LogLevel;


class Logger
{
public:
	Logger();
	~Logger();

	void   startLogging();
	void   stopLogging();
    void   write(std::string logEntry, LogLevel llevel);
	void   setLogFileSize(int flsz);
	void   setLogDirectory(std::string dirpath);
	void   setModuleName(std::string mname);
	static Logger*  GetInstance();
private:
	std::string		_LogFilename;
	std::string		_LogDirectory;
	int				_LogFileSize;
	std::string		_ModuleName;
	FILE*			_LogFile;
};

#endif

