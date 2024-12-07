#include "log.h"

#include <stdio.h>
#include <time.h>

#include "..\list\list.h"

static FILE* open_where(int destination)
{
    if (destination == CONSOLE) return stdout;
    if (destination == LOG_FILE) return fopen("log\\log.txt", "a");
    return NULL;
}

static void print_cur_time(FILE* logFile)
{
    time_t curTime = time(NULL);
    struct tm* now = localtime(&curTime);

    fprintf(logFile, "[ %d.%d.%d, %d:%d:%d ]\n", 
        now->tm_mday, now->tm_mon, now->tm_year + 1900,
        now->tm_hour, now->tm_min, now->tm_sec);
}

static void print_log_type(FILE* logFile, int logType, const char* where, const char* function)
{
    if (logType == LOG_ERR) fprintf(logFile, "ERROR in file: %s\n", where);
    if (logType == LOG_INF) fprintf(logFile, "INFORMATION in file: %s\n", where);
    fprintf(logFile, "FUNCTION: %s\n", function);
}

void log_debug(int logType, const char* where, const char* function, const char* message, int destination)
{
    FILE* logFile = open_where(destination);

    print_cur_time(logFile);
    print_log_type(logFile, logType, where, function);

    fprintf(logFile, "MESSAGE:\n%s\n\n", message);

    if (destination == LOG_FILE) fclose(logFile);
}

void log_list(int logType, const char* where, const char* function, ListPtr list, int destination)
{
    FILE* logFile = open_where(destination);

    print_cur_time(logFile);
    print_log_type(logFile, logType, where, function);

    fprintf(logFile, "List size: %lld\tList content:\n", list->size);
    int index = 0;
    for (FOR_RANGE(i, *list))
    {
#ifndef _LIST_DEBUG
        fprintf(logFile, "\t[%d] SN: %d, FN: %d, dirName: %s, engName: %s, energyPlan: %.2f, energyReal: %.2f\n",
            index++,
            i->data.serialNumber, i->data.factoryNumber,
            i->data.directorFullName, i->data.engineerFullName,
            i->data.energyConsPlan, i->data.energyConsReal);
#else
        fprintf(logFile, "\t[%d] %d\n",
            index++, i->data);
#endif
    }
    fprintf(logFile, "\n");
    fclose(logFile);
}
