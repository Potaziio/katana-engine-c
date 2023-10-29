#include "logger.h"

void logger_log_string(enum LOG_TYPE type, const char* log)
{
	switch (type)
	{
		case ERROR:
			fprintf(stderr, "%s%s[%s] ->%s %s",  COLOR_START, LOG_RED, "ERROR", COLOR_END, log);
			break;              
		case WARNING:           
			printf("%s%s[%s] ->%s %s",  COLOR_START, LOG_YELLOW, "WARNING", COLOR_END, log);
			break;              
		case SUCCESS:           
			printf("%s%s[%s] ->%s %s",  COLOR_START, LOG_GREEN, "SUCCESS", COLOR_END, log);
			break;              
		case LOG:                
			printf("%s%s[%s] ->%s %s",  COLOR_START, LOG_MAGENTA, "LOG", COLOR_END, log);
			break;
	}
}

void logger_log_float(enum LOG_TYPE type, const float log)
{
	switch (type)
	{
		case ERROR:
			fprintf(stderr, "%s%s[%s] ->%s %f\n",  COLOR_START, LOG_RED, "ERROR", COLOR_END, log);
			break;              
		case WARNING:           
			printf("%s%s[%s] ->%s %f\n",  COLOR_START, LOG_YELLOW, "WARNING", COLOR_END, log);
			break;              
		case SUCCESS:           
			printf("%s%s[%s] ->%s %f\n",  COLOR_START, LOG_GREEN, "SUCCESS", COLOR_END, log);
			break;              
		case LOG:                
			printf("%s%s[%s] ->%s %f\n",  COLOR_START, LOG_MAGENTA, "LOG", COLOR_END, log);
			break;
	}
}

void logger_log_int(enum LOG_TYPE type, const int log)
{

	switch (type)
	{
		case ERROR:
			fprintf(stderr, "%s%s[%s] ->%s %d\n",  COLOR_START, LOG_RED, "ERROR", COLOR_END, log);
			break;              
		case WARNING:           
			printf("%s%s[%s] ->%s %d\n",  COLOR_START, LOG_YELLOW, "WARNING", COLOR_END, log);
			break;              
		case SUCCESS:           
			printf("%s%s[%s] ->%s %d\n",  COLOR_START, LOG_GREEN, "SUCCESS", COLOR_END, log);
			break;              
		case LOG:                
			printf("%s%s[%s] ->%s %d\n",  COLOR_START, LOG_MAGENTA, "LOG", COLOR_END, log);
			break;
	}
}
