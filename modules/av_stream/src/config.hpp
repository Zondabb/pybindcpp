
#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <string>

#define INFO_LOG(format, ...) \
do { \
    fprintf(stderr, "[%s] " format "\n",    \
                   timeString(),          \
                   ##__VA_ARGS__);        \
} while(0)

char * timeString();

#endif // __CONFIG_HPP__