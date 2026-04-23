/**
 * @file main.c
 * Trivialny priklad pouzitia osal a xui kniznic.
 * Caka na nacitanie magstripe karty a vypise informacie o trackoch (aj ich vytlaci)
 * arm-none-linux-gnueabi-gcc -fPIC -nostartfiles -shared -include prolin_printf.h main.c -o osal_test.s
 */

#include <stdlib.h>
#include "osal.h"
#include "xui.h"

int _init()
{

    char *xui_argv[] = {"ROTATE=90","STATUSBAR=32"};
	XuiOpen(sizeof(xui_argv)/sizeof(xui_argv[0]), xui_argv);


    XuiWindow *root;
	root = XuiRootCanvas();

	XuiFont *font;
	font = XuiCreateFont("/usr/font/tm_font", 0, XUI_FONT_GREY);
	if(font == NULL) {
		printf("XuiCreateFont error!\n");
	}

    XuiWindow *statusbar;
	  statusbar = XuiStatusbarCanvas();
	  XuiCanvasSetBackground(statusbar, XUI_BG_NORMAL, NULL, XuiColor(255, 0, 255, 255));
    XuiCanvasSetBackground(root, XUI_BG_NORMAL, NULL, XuiColor(255, 255, 255, 255));


    ST_MSR_DATA Track1, Track2, Track3;

    OsMsrOpen();

    XuiClearArea(root, 0, 0, 239, 288);
	  XuiCanvasDrawText(root, (240-XuiTextWidth(font, 5, "Cakam na kartu...")) / 2, 140, 5, font, 0, XuiColor(255, 0, 0, 0), "Cakam na kartu...");
	  XuiCanvasDrawText(statusbar, (240-XuiTextWidth(font, 1, "Magstripe Reader")) / 2, 25, 1, font, 0, XuiColor(255, 0, 0, 0), "Magstripe Reader");	
	
    // cakame doḱym neprejdeme kartou
    while (OsMsrSwiped() == 0) {}


    OsMsrRead(&Track1, &Track2, &Track3);
    OsPrnOpen(0, NULL);
    OsPrnSelectFontSize(16, 24, 16, 24);

    if (Track1.Status == 0) {
        OsPrnPrintf("\nTrack 1: \n");
        OsPrnPrintf((char*)Track1.TrackData);
    } else {
        OsPrnPrintf("Track 1: Chyba\n");
    }

    if (Track2.Status == 0) {
        OsPrnPrintf("\nTrack 2: \n");
        OsPrnPrintf((char*)Track2.TrackData);  
    } else {
        OsPrnPrintf("Track 2: Chyba\n");
    }

    if (Track3.Status == 0) {
        OsPrnPrintf("\nTrack 3: \n");
        OsPrnPrintf((char*)Track3.TrackData);  
    } else {
        OsPrnPrintf("Track 3: Chyba\n");
    }

    XuiClearArea(root, 0, 0, 239, 288);
    XuiCanvasDrawText(root, (240-XuiTextWidth(font, 5, "Karta nacitana")) / 2, 140, 5, font, 0, XuiColor(255, 0, 0, 0), "Karta nacitana");
    
    // jednotlive tracky na obrazovku
    XuiCanvasDrawText(root, 0, 0, 2, font, 0, XuiColor(255, 0, 0, 0), (char*)Track1.TrackData);
    XuiCanvasDrawText(root, 0, 40, 2, font, 0, XuiColor(255, 0, 0, 0), (char*)Track2.TrackData);
    XuiCanvasDrawText(root, 0, 80, 2, font, 0, XuiColor(255, 0, 0, 0), (char*)Track3.TrackData);
    OsPrnSetGray(4);
    OsPrnStart();

    // pockame 5 sekund
    sleep(5);

    OsPrnClose();
    OsMsrClose();
    XuiDestroyFont(font);
	  XuiClose();
    exit(0);
}
