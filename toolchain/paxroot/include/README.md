# Toolchain headers

### xui.h
half baked and handmade header file for nearly all libxui.so relevant functions. For (partial) usage, look at xui_test.c

### osal.h
header file for all libosal.so relevant functions. For usage, look at osal_test.c

## Deprecated headers, but still included for historical reasons

### prolin-printf.h
header file use as redefinition of printf with flush to stdout.

`-include prolin-printf.h` in cflags for global usage across projects
