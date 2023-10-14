#ifndef PLATFORM_DEFINITIONS
#define PLATFORM_DEFINITIONS

// C headers
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

// C++ Headers
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <map>
#include <list>
#include <cctype>
#include <ctime>
#include <ios>
#include <fstream>
#include <algorithm>
#include <bitset>
#include <deque>
#include <iomanip>
#include <functional>
#include <filesystem>
#include <thread>
#include <mutex>

#if defined(_WIN32) || defined(_WIN64)//Windows includes
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <WinBase.h>
#include <Windows.h>
#include <process.h>
#include <direct.h>
#include <TlHelp32.h>
#include <malloc.h>
#endif
#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pwd.h>
#endif

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#define LAST_ERROR    ::GetLastError()
#define SOCK_ERROR    ::WSAGetLastError()
#define strtoull(str, endptr, base) _strtoui64(str, endptr, base)
#define threadsleep(n) ::Sleep(n*1000)
#define getpid() _getpid()
#define pid_t    long
#define strcasecmp _strcmpi
#else
#define SOCKET int
#define LAST_ERROR    errno
#define SOCK_ERROR    errno
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define LPVOID void*
#define DWORD	unsigned long
#define WINAPI	__stdcall
#define threadsleep(n) sleep(n)
#define LPARAM	long
#define WPARAM	unsigned int
#define closesocket(n) close(n)
#endif

#endif
