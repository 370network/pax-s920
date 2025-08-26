#include "ui.h"
#include "app.h"
#include "applist.h"
#include <stdio.h>
#include <osal.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "apploader.h"

XuiWindow *root;
XuiFont *font;

void initui(AppList *applist)
{
    char *xui_argv[] = {"ROTATE=90", "STATUSBAR=32"};
    XuiOpen(sizeof(xui_argv) / sizeof(xui_argv[0]), xui_argv);

    root = XuiRootCanvas();
    font = XuiCreateFont("/usr/font/tm_font", 0, XUI_FONT_GREY);

    XuiWindow *statusbar;
    statusbar = XuiStatusbarCanvas();
    XuiCanvasSetBackground(statusbar, XUI_BG_NORMAL, 0, XuiColor(0, 128, 0, 0));
    XuiCanvasSetBackground(root, XUI_BG_NORMAL, 0, XuiColor(255, 255, 255, 255));
    
    // Battery and ethernet status in future? 
    // XuiSetStatusbarIcon(5, "/usr/share/tm/tm_icon_pageup.bmp");
	// XuiSetStatusbarIcon(7, "/usr/share/tm/tm_icon_pagedown.bmp");


    char *statusbar_text = "PaxBoard";
    XuiCanvasDrawText(statusbar, 5, 15, 10, font, 0, XuiColor(255, 255, 255, 255), statusbar_text);

    XuiClearArea(root, 0, 0, 239, 288);

    if (applist && applist->count > 0) {
        for (int i = 0; i < applist->count; i++) {
            if (applist->apps[i].name) {
                printf("Adding app: %s\n", applist->apps[i].name);

                char icon_path[256];
                snprintf(icon_path, sizeof(icon_path), "/data/app/MAINAPP/apps/%s/icon.png", applist->apps[i].app);
                XuiImg *app_icon = XuiImgLoadFromFile(icon_path);

                if (!app_icon) {
                    printf("Failed to load icon for app: %s\n", applist->apps[i].name);
                    continue;
                }

                int col = i % 4; 
                int row = i / 4; 
                unsigned int x = 10 + col * 60;
                unsigned int y = 20 + row * 60;


                
                XuiCanvasDrawImg(root, x, y, 48, 48, XUI_BG_NORMAL, app_icon);
                XuiWindow *btn = XuiCreateButton(root, x, y, 48, 48);
            

                if (!btn) {
                    printf("Failed to create button");
                    continue;
                }

                
                XuiButtonStat* btn_stat = malloc(sizeof(XuiButtonStat));
                if (!btn_stat) {
                    printf("malloc fail");
                    continue;
                }


                // I couldn't find a reason why img is not working, so I'm rendering an image bellow this and making this button transparent
                // If anyone has nerves to fix it, please do so

                *btn_stat = (XuiButtonStat){
                    .btn_bg = XuiColor(0, 255, 0, 255),
                    .img = *app_icon
                };


                if (XuiButtonSetStat(btn, XUI_BTN_NORMAL, btn_stat) != 0) {
                    printf("Failed to set button stat");
                    free(btn_stat);
                    continue;
                }

                XuiButtonSetKey(btn, i + 16);
                XuiShowWindow(btn, 1, 0);
               
            }
        }
    } else {
        XuiCanvasDrawText(root, (240 - XuiTextWidth(font, 5, "No Apps")) / 2, 140, 5, font, 0, XuiColor(255, 0, 0, 0), "No Apps");
    }


    while (1){
        int key = XuiGetKey();
            if (key >= 16 && key < 16 + applist->count) {
                int app_index = key - 16;
                printf("Launching app: %s\n", applist->apps[app_index].name);
                XuiClose();
                LoadApp(applist->apps[app_index].app);
                initui(applist);
        }
    }
}