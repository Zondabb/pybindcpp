#include <time.h>
#include <stdio.h>

#include "config.hpp"

char * timeString() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm * timeinfo = localtime(&ts.tv_sec);
    static char timeStr[40];
    sprintf(timeStr, "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3ld+08:00",
            timeinfo->tm_year + 1900,
            timeinfo->tm_mon + 1,
            timeinfo->tm_mday,
            timeinfo->tm_hour,
            timeinfo->tm_min,
            timeinfo->tm_sec,
            ts.tv_nsec / 1000000);
    return timeStr;
}
