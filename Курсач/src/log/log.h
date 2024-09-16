#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#include "..\list\list.h"

enum LogType
{
	ERR,
	INF
};

enum LogDestination
{
	CONSOLE,
	LOG_FILE
};

void log(int logType, const char* where, const char* function, const char* message, int destination);
void log_list(int logType, const char* where, const char* function, ListPtr list, int destination);

#ifdef _DEBUG
	#define LOG(logType, where, function, message, destination) log(logType, where, function, message, destination);
#else
	#define LOG(logType, where, function, message, destination)
#endif

#ifdef _DEBUG
	#define LOG_LIST(logType, where, function, list, destination) log_list(logType, where, function, list, destination);
#else
	#define LOG_LIST(logType, where, function, list, destination)
#endif

#define LOG_RELEASE(logType, where, function, message, destination) log(logType, where, function, message, destination);
#define LOG_LIST_RELEASE(logType, where, function, list, destination) log_list(logType, where, function, list, destination);

#endif