#ifndef PAX_PRINTF
#define PAX_PRINTF

#include <stdio.h>

#define printf(format, ...) \
    (printf(format, ##__VA_ARGS__), fflush(stdout))

#endif
