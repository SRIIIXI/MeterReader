#include "Logger.hpp"
#include <stdio.h>
#include <time.h>

#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
#include <Windows.h>
#include <memory.h>
#include <process.h>
#include <direct.h>
#define createdir(str) _mkdir(str)
#define getcwd(ptr,n) _getcwd(ptr,n)
#define chdir(str) _chdir(str)
#define getpid() _getpid()
#else
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#define createdir(str) mkdir(str,S_IRWXU)
#endif

typedef std::map<LogLevel, std::string> LogLevelStrings;
LogLevelStrings	_LogLevelMap;

Logger* objLogger = nullptr;

void createDirectory(const char* str);
bool directoryExists(const char* ptr);

Logger*  Logger::GetInstance()
{
	if (objLogger == nullptr)
	{
		objLogger = new Logger();
	}

	return objLogger;
}

Logger::Logger()
{
	_LogDirectory = "";
	_LogFileSize = 1024;

    char pidstr[17] = {0};
	memset((char*)&pidstr[0], 0, 16);
    snprintf(pidstr, 16, "%d", getpid());
	_ModuleName = pidstr;

	_LogLevelMap.clear();

	LogLevel l1 = LogInfo;
	LogLevel l2 = LogWarning;
	LogLevel l3 = LogError;
	LogLevel l4 = LogCritical;

	_LogLevelMap[l1]= "Information";
	_LogLevelMap[l2] = "Warning    ";
	_LogLevelMap[l3] = "Error      ";
	_LogLevelMap[l4] = "Critical   ";
}

Logger::~Logger()
{
	fflush(_LogFile);
	stopLogging();
	_LogLevelMap.clear();
}

void Logger::stopLogging()
{
	if (_LogFile)
	{
		fclose(_LogFile);
	}
}

void Logger::startLogging()
{
	if (_LogDirectory.length() < 1)
	{
		char filepathbuffer[1024];
		memset((char*)&filepathbuffer[0], 0, 1024);
		getcwd(&filepathbuffer[0], 1024);
		_LogDirectory = filepathbuffer;
		_LogDirectory += "/log/";

		if (!directoryExists(_LogDirectory.c_str()))
		{
			createDirectory(_LogDirectory.c_str());
		}
	}

	_LogFilename = _LogDirectory + _ModuleName + ".log";

	_LogFile = fopen(_LogFilename.c_str(), "w");
}

void Logger::write(std::string logEntry, LogLevel llevel)
{
	if (_LogFile)
	{
		int sz = ftell(_LogFile);

        //Max 100MB
        if (sz >= _LogFileSize * 1024 * 100)
		{
			stopLogging();
			startLogging();
		}

		std::string lvel = _LogLevelMap[llevel];

        char tstamp[33] = {0};
		time_t t;
		struct tm* tmp;
		time(&t);
		tmp = localtime(&t);

        snprintf(tstamp, 33, "%02d.%02d.%04d-%02d.%02d.%02d", tmp->tm_mday, (tmp->tm_mon + 1), (tmp->tm_year + 1), tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

        char temp[1025] = {0};
		memset((char*)&temp[0], 0, 16);
	
        snprintf(temp, 1024, "%s|%s|%s\n", tstamp, lvel.c_str(), logEntry.c_str());

		fwrite(temp, strlen(temp), 1, _LogFile);
		fflush(_LogFile);

		printf("%s", temp);
		fflush(stdout);
	}
}

void Logger::setModuleName(std::string mname)
{
	_ModuleName = mname;
}

void Logger::setLogFileSize(int flsz)
{
	_LogFileSize = flsz;
}

void Logger::setLogDirectory(std::string dirpath)
{
	_LogDirectory = dirpath;
	_LogDirectory += "/";

	if (_LogDirectory.length() > 0)
	{
		if (!directoryExists(_LogDirectory.c_str()))
		{
			createDirectory(_LogDirectory.c_str());
		}
	}
}

bool directoryExists(const char* ptr)
{
#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)

	HANDLE hfind = 0;
	WIN32_FIND_DATAA dstruct;
	hfind = FindFirstFileA(ptr, &dstruct);

	if (hfind == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	FindClose(hfind);
	return true;

#else

	DIR* dirp;

	dirp = opendir(ptr);
	if (dirp == NULL)
	{
		return false;
	}
	closedir(dirp);
	return true;

#endif
}

void createDirectory(const char* str)
{
	createdir(str);
}
