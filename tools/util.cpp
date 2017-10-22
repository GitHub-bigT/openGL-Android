#include "mlt.h"

#ifdef _WIN32
#include <time.h>
#include <Synchapi.h>

void timer_start(FILETIME *start)
{
    GetSystemTimeAsFileTime(start);
}

int64_t timer_elapsed_msec(FILETIME *start)
{
    FILETIME now;
    GetSystemTimeAsFileTime(&now);
    int64_t elapsed = ((((int64_t)now.dwHighDateTime) << 32) + (int64_t)now.dwLowDateTime) -
            ((((int64_t)start->dwHighDateTime) << 32) + (int64_t)start->dwLowDateTime);
    return elapsed / 10000;
}

int64_t timer_elapsed_usec(FILETIME *start)
{
    FILETIME now;
    GetSystemTimeAsFileTime(&now);
    int64_t elapsed = ((((int64_t)now.dwHighDateTime) << 32) + (int64_t)now.dwLowDateTime) -
            ((((int64_t)start->dwHighDateTime) << 32) + (int64_t)start->dwLowDateTime);
    return elapsed / 10;
}
#else
#include <sys/time.h>

void timer_start(struct timeval *start)
{
    gettimeofday(start, NULL);
}

int64_t timer_elapsed_msec(struct timeval *start)
{
    struct timeval now;
    gettimeofday(&now, NULL);

    int64_t elapsed = ( ( int64_t )now.tv_sec * 1000000 + now.tv_usec ) -
            ( ( int64_t )start->tv_sec * 1000000 + start->tv_usec );
    return elapsed / 1000;
}

int64_t timer_elapsed_usec(struct timeval *start)
{
    struct timeval now;
    gettimeofday(&now, NULL);

    int64_t elapsed = ( ( int64_t )now.tv_sec * 1000000 + now.tv_usec ) -
            ( ( int64_t )start->tv_sec * 1000000 + start->tv_usec );
    return elapsed;
}
#endif
