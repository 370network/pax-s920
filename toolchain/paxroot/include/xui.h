/**
 * @file xui.h
 * @brief Header for Prolin XUI GUI Interface.
 *
 * This file was manually assembled based on the Prolin XUI Interface V2.1.7. It is incomplete and may contain errors.
 * Authors:
 * - Milan Babuljak (hiimmilan.dev), 2024
 * - Richard Gráčik (Morc), 2024
 */


#ifndef PROLIN_XUI_H
#define PROLIN_XUI_H

/*--------------------------------------------
 * Macros and Enums
 *-------------------------------------------*/
#define XuiColor(b, g, r, a) ( b  << 24 | g << 16 | r << 8 | (a) )

typedef enum {
    XUI_ROTATE_0 = 0,
    XUI_ROTATE_90 = 1,
    XUI_ROTATE_180 = 2,
    XUI_ROTATE_270 = 3,
    XUI_FLIP_VERT = 4,
    XUI_FLIP_HORIZ = 5
} XuiTransform;

typedef enum {
    XUI_BTN_NORMAL = 0,
    XUI_BTN_PRESSED = 1
} XuiButtonStatType;

typedef enum {
    XUI_BG_NORMAL = 0,
    XUI_BG_TILE = 1,
    XUI_BG_CENTER = 2,
    XUI_BG_FOUR_CORNER = 3
} XuiBgStyle;

typedef enum {
    XUI_FONT_MONO = 0,
    XUI_FONT_GREY = 1
} XuiFontSet;

typedef enum {
    XUI_TEXT_NORMAL = 0,
    XUI_BOLD = 1,
    XUI_ITALIC = 2,
    XUI_TEXT_BOLD_ITALIC = 3
} XuiTextStyle;

typedef enum {
    XUI_WIN_CANVAS = 0,
    XUI_WIN_BUTTON = 1,
    XUI_WIN_GIF = 2,
    XUI_WIN_SIGBOARD = 3
} XuiWindowType;

#define XUI_KEY1 2
#define XUI_KEY9  10
#define XUI_KEY0  11
#define XUI_KEYCANCEL 223
#define XUI_KEYCLEAR 14
#define XUI_KEYENTER 28
#define XUI_KEYFUNC 102
#define XUI_KEYALPHA 69

/*--------------------------------------------
 * Structures
 *-------------------------------------------*/
typedef struct {
	int width;
	int height;
	int *widget;
	XuiWindowType type;
	int key;
} XuiWindow;

typedef struct {
	char *fontfile;
	int index;
	XuiFontSet fontset;
} XuiFont;

typedef struct {
	int width;
	int height;
	int *prly;
} XuiImg;

typedef struct {
	int btn_round;
	int btn_bg;
	char *text;
	int text_fg;
	XuiFont text_font;
	int text_x;
	int text_y;
	int text_height;
	XuiImg img;
	int img_x;
	int img_y;
} XuiButtonStat;

typedef struct {
	int btn_round;
	int btn_bg;
	char text;
	int text_fg;
	XuiFont text_font;
	int text_height;
	XuiImg *img;
	int img_x;
	int img_y;
	int img_style;
	int pen_fg;
	int pen_width;
	int pen_flat;
} XuiSigBoardStat;

typedef struct {
	char text;
	int text_fg;
	XuiFont text_font;
	int text_x;
	int text_y;
	int text_height;
	XuiImg *img;
	int img_x;
	int img_y;
	int img_style;
} XuiCameraStat;

typedef struct {
	XuiWindow *parent;
	int x;
	int y;
	int width;
	int height;
	XuiFont text_font;
	int text_size;
	int text_fg;
	int focus_fg;
	XuiImg img;
	int img_bg;
	int alpha_key;
	int sharp_key;
} XuiImeAttr;

typedef struct {
	XuiWindow *parent;
	int x;
	int y;
	XuiFont font;
	int size;
	int fg;
	int alpha_key;
} XuiGetStrAttr;

typedef struct {
	int point_array;
	int point_len;
} XuiSignData;

typedef struct {
	int type;
	int velocity;
	int distance;
	int down_x;
	int down_y;
	int cur_x;
	int cur_y;
} XuiGesture;

/*--------------------------------------------
 * Function Prototypes - System
 *-------------------------------------------*/
int XuiOpen(int argc, char** argv);
int XuiIsRunning(void);
void XuiClose(void);
int XuiSuspend(void);
int XuiResume(void);
XuiWindow *XuiRootCanvas(void);
XuiWindow *XuiStatusbarCanvas(void);

/*--------------------------------------------
 * Function Prototypes - Create
 *-------------------------------------------*/
XuiFont *XuiCreateFont(char *fontFile, int index, XuiFontSet fontset);
XuiWindow *XuiCreateCanvas(XuiWindow *parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
XuiWindow *XuiCreateCanvasEx(XuiWindow *parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int vh);
XuiWindow *XuiCreateButton(XuiWindow *parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
XuiWindow *XuiCreateSignatureBoard(XuiWindow *parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
XuiWindow *XuiCreateGif(XuiWindow *parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height, const char* path);
XuiWindow *XuiCreateCamera(XuiWindow *parent, unsigned int x, unsigned int y, int index, int resolution, int zoom);
XuiWindow *XuiCreateScaner(XuiWindow *parent, unsigned int x, unsigned int y, int index, int zoom);

/*--------------------------------------------
 * Function Prototypes - Destroy
 *-------------------------------------------*/
void XuiDestroyFont(XuiFont *font);
void XuiDestroyWindow(XuiWindow *window);

/*--------------------------------------------
 * Function Prototypes - Util
 *-------------------------------------------*/
int XuiTextWidth(XuiFont *font, int size, char *text);
int XuiTextWidthEx(XuiFont *font, int size, XuiTextStyle textstyle, char *text);
void XuiShowWindow(XuiWindow *window, int show, int flag);
int XuiSetStatusbarIcon(int index, const char* path);
int XuiGetHzString(XuiImeAttr attr, char *outstr, unsigned int maxlen, unsigned int timeout);
int XuiGetString(XuiGetStrAttr attr, char *outstr, unsigned char mode, int minlen, int maxlen);
char *XuiBidiStrdup(const char *str);
int XuiGetGesture(XuiGesture *gesture);
int XuiSetGestureRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
int XuiShowSoftKeyboard(int type, int show);
//XuiTsUpdate

/*--------------------------------------------
 * Function Prototypes - Button
 *-------------------------------------------*/
int XuiButtonSetStat(XuiWindow *window, XuiButtonStatType type, XuiButtonStat *stat);
int XuiButtonSetKey(XuiWindow *window, int key);
int XuiHasKey(void);
int XuiGetKey(void);
//XuiButtonRegisterKey

/*--------------------------------------------
 * Function Prototypes - SigBoard
 *-------------------------------------------*/
int XuiSigBoardSetStat(XuiWindow *window, XuiSigBoardStat *stat);
XuiImg *XuiSigBoardImg(XuiWindow *window);
XuiSignData *XuiSigBoardSignData(XuiWindow *window);

/*--------------------------------------------
 * Function Prototypes - Camera
 *-------------------------------------------*/
int XuiCameraSetStat(XuiWindow *window, XuiCameraStat *stat);
XuiImg *XuiCameraCapture(XuiWindow *window);
int XuiScanerDecode(XuiWindow *window, unsigned char *outdata, int *datalen, int maxlen, int timeout);

/*--------------------------------------------
 * Function Prototypes - Canvas
 *-------------------------------------------*/
int XuiCanvasDrawText(XuiWindow *window, unsigned int x, unsigned int y, unsigned int height, XuiFont *font, XuiTextStyle textstyle, long fg, char *text);
int XuiCanvasDrawTextEx(XuiWindow *window, unsigned int x, unsigned int y, unsigned int height, XuiFont *font, XuiTextStyle textstyle, long fg, char *text, int linebreak);
int XuiCanvasDrawImg(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, XuiBgStyle bgstyle, XuiImg *img);
int XuiCanvasDrawRect(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, int fg, int round, int fill);
void XuiCanvasMoveToY(XuiWindow *window, unsigned int my);
void XuiCanvasSetBackground(XuiWindow *window, XuiBgStyle bgstyle, XuiImg *img, long bg);
int XuiCanvasAnimation(XuiWindow *front, XuiWindow *background, unsigned int front_rate, unsigned int background_rate, int type);

/*--------------------------------------------
 * Function Prototypes - Capture
 *-------------------------------------------*/
XuiImg *XuiCaptureScreen(void);
XuiImg *XuiCaptureCanvas(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

/*--------------------------------------------
 * Function Prototypes - Clear
 *-------------------------------------------*/
int XuiClearArea(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
void XuiClearKey(void);
void XuiClearGesture(void);

/*--------------------------------------------
 * Function Prototypes - Image 
 *-------------------------------------------*/
XuiImg *XuiImgLoadFromFile(const char *file);
XuiImg *XuiImgLoadFromMem(unsigned char *address, unsigned long length, int type);
int XuiImgSaveToFile(XuiImg *img, const char *file);
int XuiImgToRgba(XuiImg *img, char *rgba);
int XuiImgToGray8(XuiImg *img, const char *gray8);
int XuiImgToBgr24(XuiImg *img, char *bgr24);
int XuiImgToFrameBuffer(XuiImg *img, unsigned char *data, unsigned int size);
int XuiImgTransform(XuiImg *img, XuiTransform transform);
int XuiImgResize(XuiImg *img, unsigned int width, unsigned int height);
XuiImg *XuiImgCompose(XuiImg *img1, XuiImg *img2, unsigned int rate1, unsigned int rate2, int type);
void XuiImgFree(XuiImg *img);
XuiImg *XuiRgbaToImg(char *rgba, unsigned int len, unsigned int width, unsigned int height);
XuiImg *XuiFrameBufferToImg(char *data, unsigned int len, unsigned int width, unsigned int height);

#endif // PROLIN_XUI_H
