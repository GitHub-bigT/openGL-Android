#pragma once
#include <windows.h>
#include <stdint.h>

#define MLT_TIMER FILETIME
void timer_start(FILETIME *start);
int64_t timer_elapsed_msec(FILETIME *start);
int64_t timer_elapsed_usec(FILETIME *start);