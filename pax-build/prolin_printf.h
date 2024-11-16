#ifndef PROLIN_PRINTF_H
#define PROLIN_PRINTF_H

#include <stdio.h>

#define printf(format, ...) \
    (printf(format, ##__VA_ARGS__), fflush(stdout))

#endif
