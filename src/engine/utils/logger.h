#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#define COLOR_START "\033[1;"
#define COLOR_END "\033[0m"

#define LOG_RED "31m"
#define LOG_YELLOW "33m"
#define LOG_GREEN "32m"
#define LOG_MAGENTA "35m"

enum LOG_TYPE
{
	ERROR,
	WARNING,
	SUCCESS,
	LOG,
};

void logger_log_string(enum LOG_TYPE type, const char* log);
void logger_log_float(enum LOG_TYPE type, const float log);
void logger_log_int(enum LOG_TYPE type, const int log);

#endif
