#include "xui.h"

int xui_result = 0;

int _init()
{
	char *xui_argv[] = {"ROTATE=90","STATUSBAR=32"};
	XuiOpen(sizeof(xui_argv)/sizeof(xui_argv[0]), xui_argv);
	
	//XuiOpen(0, NULL); //landscape, no statusbar

	XuiImg *bg;
	bg = XuiImgLoadFromFile("/usr/share/tm/logo32.png");

	XuiWindow *root;
	root = XuiRootCanvas();

	XuiFont *font;
	font = XuiCreateFont("/usr/font/tm_font", 0, XUI_FONT_GREY);
	if(font == NULL)
	{
		printf("XuiCreateFont error!\n");
	}


	//statusbar + eight 19x9 icons
	XuiWindow *statusbar;
	statusbar = XuiStatusbarCanvas();
	XuiCanvasSetBackground(statusbar, XUI_BG_NORMAL, NULL, XuiColor(255, 0, 255, 255));
	XuiSetStatusbarIcon(0, "/usr/share/tm/tm_icon_pageup.bmp");
	XuiSetStatusbarIcon(1, "/usr/share/tm/tm_icon_pagedown.bmp");
	XuiSetStatusbarIcon(2, "/usr/share/tm/tm_icon_right.bmp");
	XuiSetStatusbarIcon(3, "/usr/share/tm/tm_icon_left.bmp");
	XuiSetStatusbarIcon(4, "/usr/share/tm/tm_icon_up.bmp");
	XuiSetStatusbarIcon(5, "/usr/share/tm/tm_icon_down.bmp");
	XuiSetStatusbarIcon(6, "/usr/share/tm/tm_icon_pageup.bmp");
	XuiSetStatusbarIcon(7, "/usr/share/tm/tm_icon_pagedown.bmp");

	XuiCanvasSetBackground(root, XUI_BG_NORMAL, bg, XuiColor(255, 255, 255, 255));
	usleep(250000);
	XuiCanvasSetBackground(root, XUI_BG_TILE, bg, XuiColor(255, 255, 255, 255));
	usleep(250000);
	XuiCanvasSetBackground(root, XUI_BG_CENTER, bg, XuiColor(255, 255, 255, 255));
	usleep(250000);
	XuiCanvasSetBackground(root, XUI_BG_FOUR_CORNER, bg, XuiColor(255, 255, 255, 255));
	usleep(250000);
	XuiCanvasSetBackground(root, XUI_BG_NORMAL, NULL, XuiColor(255, 255, 255, 255));
	
	XuiClearArea(root, 0, 0, 239, 288);
	xui_result = XuiCanvasDrawText(root, (240-XuiTextWidth(font, 5, "Xui testuvacka")) / 2, 140, 5, font, 0, XuiColor(255, 0, 0, 0), "Xui testuvacka");
	if(xui_result != 0)
	{
		printf("XuiCanvasDrawText error: %d\n", xui_result);
	}
	
	char* statusbar_text = "370network";
	XuiCanvasDrawText(statusbar, (240-XuiTextWidth(font, 1, statusbar_text)) / 2, 25, 1, font, 0, XuiColor(255, 0, 0, 0), statusbar_text);	
	


	//XuiShowSoftKeyboard(0, 1);

	//XuiShowWindow(root, 1, 0);
	sleep(1);

	XuiDestroyFont(font);
	XuiClose();
	
	return 0;
}
