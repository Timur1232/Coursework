﻿#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#include "..\list\list.h"

enum LogType
{
    LOG_ERR,
    LOG_INF
};

enum LogDestination
{
    CONSOLE,
    LOG_FILE
};

void log_debug(int logType, const char* where, const char* function, const char* message, int destination);
void log_list(int logType, const char* where, const char* function, ListPtr list, int destination);

#ifdef _DEBUG
    #define LOG_DEBUG(logType, where, function, message, destination) log_debug(logType, where, function, message, destination);
#else
    #define LOG_DEBUG(logType, where, function, message, destination) 
#endif

#ifdef _DEBUG
    #define LOG_LIST(logType, where, function, list, destination) log_list(logType, where, function, list, destination);
#else
    #define LOG_LIST(logType, where, function, list, destination) 
#endif

#define LOG_RELEASE(logType, where, function, message, destination) log_debug(logType, where, function, message, destination);
#define LOG_LIST_RELEASE(logType, where, function, list, destination) log_list(logType, where, function, list, destination);

#endif