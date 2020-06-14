#include "time.h"
#include "stdio.h"
#include "ls_util.h"

long long GetCurrentTime()
{
	struct timespec tm;
	clock_gettime(CLOCK_MONOTONIC, &tm);
	return tm.tv_nsec + tm.tv_sec * 1000000000;
}

void PrintTimeFormat(FILE* stream)
{
        char buffer[64];
        time_t t = time(NULL);
        struct tm lt;
        localtime_r(&t, &lt);
	strftime(buffer, sizeof(buffer), "%F %T", &lt);
        fprintf(stream, "%s", buffer);
}
