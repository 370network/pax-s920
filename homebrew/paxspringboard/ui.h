#ifndef UI_H
#define UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <xui.h>
#include "applist.h"

typedef enum {
    UI_RESULT_NONE = 0,
    UI_RESULT_RELAUNCH = 1,
    UI_RESULT_EXIT = 2,
    UI_RESULT_SUSPEND = 3,
} UIResult;

UIResult initui(AppList *applist);

#ifdef __cplusplus
}
#endif

#endif // UI_MANAGER_H
