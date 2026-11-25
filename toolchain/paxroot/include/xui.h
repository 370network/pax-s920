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
#define XUI_KEY2 3
#define XUI_KEY3 4
#define XUI_KEY4 5
#define XUI_KEY5 6
#define XUI_KEY6 7
#define XUI_KEY7 8
#define XUI_KEY8 9
#define XUI_KEY9  10
#define XUI_KEY0  11
#define XUI_KEYCANCEL 223
#define XUI_KEYCLEAR 14
#define XUI_KEYENTER 28
#define XUI_KEYFUNC 102
#define XUI_KEYALPHA 69
#define XUI_KEYUP 103
#define XUI_KEYDOWN 108

#define XUI_KEYENTER1 30
#define XUI_KEYENTER2 31
#define XUI_KEYENTER3 32
#define XUI_KEYENTER4 33
#define XUI_KEYENTER5 34
#define XUI_KEYENTER6 35
#define XUI_KEYENTER7 36
#define XUI_KEYENTER8 37
#define XUI_KEYENTER9 38
#define XUI_KEYENTER0 39

#define XUI_SOFTKEYBOARD_KEY 0xff
#define XUI_SOFTKEYBOARD_KEYBACKSPACE (XUI_SOFTKEYBOARD_KEY+0x08)

#define XUI_SOFTKEYBOARD_KEYSPACE (XUI_SOFTKEYBOARD_KEY+0x20)
#define XUI_SOFTKEYBOARD_KEYEXCLAM (XUI_SOFTKEYBOARD_KEY+0x21)
#define XUI_SOFTKEYBOARD_KEYDOUBLEQUOTE (XUI_SOFTKEYBOARD_KEY+0x22)
#define XUI_SOFTKEYBOARD_KEYSHARP (XUI_SOFTKEYBOARD_KEY+0x23)
#define XUI_SOFTKEYBOARD_KEYDOLLAR (XUI_SOFTKEYBOARD_KEY+0x24)
#define XUI_SOFTKEYBOARD_KEYPERCENT (XUI_SOFTKEYBOARD_KEY+0x25)
#define XUI_SOFTKEYBOARD_KEYAMPERSAND (XUI_SOFTKEYBOARD_KEY+0x26)
#define XUI_SOFTKEYBOARD_KEYSINGLEQUOTE (XUI_SOFTKEYBOARD_KEY+0x27)
#define XUI_SOFTKEYBOARD_KEYPARENLEFT (XUI_SOFTKEYBOARD_KEY+0x28)
#define XUI_SOFTKEYBOARD_KEYPARENRIGHT (XUI_SOFTKEYBOARD_KEY+0x29)
#define XUI_SOFTKEYBOARD_KEYASTERISK (XUI_SOFTKEYBOARD_KEY+0x2a)
#define XUI_SOFTKEYBOARD_KEYPLUS (XUI_SOFTKEYBOARD_KEY+0x2b)
#define XUI_SOFTKEYBOARD_KEYCOMMA (XUI_SOFTKEYBOARD_KEY+0x2c)
#define XUI_SOFTKEYBOARD_KEYMINUS (XUI_SOFTKEYBOARD_KEY+0x2d)
#define XUI_SOFTKEYBOARD_KEYPERIOD (XUI_SOFTKEYBOARD_KEY+0x2e)
#define XUI_SOFTKEYBOARD_KEYSLASH (XUI_SOFTKEYBOARD_KEY+0x2f)

#define XUI_SOFTKEYBOARD_KEY0 (XUI_SOFTKEYBOARD_KEY+0x30)
#define XUI_SOFTKEYBOARD_KEY1 (XUI_SOFTKEYBOARD_KEY+0x31)
#define XUI_SOFTKEYBOARD_KEY2 (XUI_SOFTKEYBOARD_KEY+0x32)
#define XUI_SOFTKEYBOARD_KEY3 (XUI_SOFTKEYBOARD_KEY+0x33)
#define XUI_SOFTKEYBOARD_KEY4 (XUI_SOFTKEYBOARD_KEY+0x34)
#define XUI_SOFTKEYBOARD_KEY5 (XUI_SOFTKEYBOARD_KEY+0x35)
#define XUI_SOFTKEYBOARD_KEY6 (XUI_SOFTKEYBOARD_KEY+0x36)
#define XUI_SOFTKEYBOARD_KEY7 (XUI_SOFTKEYBOARD_KEY+0x37)
#define XUI_SOFTKEYBOARD_KEY8 (XUI_SOFTKEYBOARD_KEY+0x38)
#define XUI_SOFTKEYBOARD_KEY9 (XUI_SOFTKEYBOARD_KEY+0x39)

#define XUI_SOFTKEYBOARD_KEYCOLON (XUI_SOFTKEYBOARD_KEY+0x3a)
#define XUI_SOFTKEYBOARD_KEYSEMICOLON (XUI_SOFTKEYBOARD_KEY+0x3b)
#define XUI_SOFTKEYBOARD_KEYLESS (XUI_SOFTKEYBOARD_KEY+0x3c)
#define XUI_SOFTKEYBOARD_KEYEQUAL (XUI_SOFTKEYBOARD_KEY+0x3d)
#define XUI_SOFTKEYBOARD_KEYGREATER (XUI_SOFTKEYBOARD_KEY+0x3e)
#define XUI_SOFTKEYBOARD_KEYQUESTION (XUI_SOFTKEYBOARD_KEY+0x3f)
#define XUI_SOFTKEYBOARD_KEYAT (XUI_SOFTKEYBOARD_KEY+0x40)

#define XUI_SOFTKEYBOARD_KEYA (XUI_SOFTKEYBOARD_KEY+0x41)
#define XUI_SOFTKEYBOARD_KEYB (XUI_SOFTKEYBOARD_KEY+0x42)
#define XUI_SOFTKEYBOARD_KEYC (XUI_SOFTKEYBOARD_KEY+0x43)
#define XUI_SOFTKEYBOARD_KEYD (XUI_SOFTKEYBOARD_KEY+0x44)
#define XUI_SOFTKEYBOARD_KEYE (XUI_SOFTKEYBOARD_KEY+0x45)
#define XUI_SOFTKEYBOARD_KEYF (XUI_SOFTKEYBOARD_KEY+0x46)
#define XUI_SOFTKEYBOARD_KEYG (XUI_SOFTKEYBOARD_KEY+0x47)
#define XUI_SOFTKEYBOARD_KEYH (XUI_SOFTKEYBOARD_KEY+0x48)
#define XUI_SOFTKEYBOARD_KEYI (XUI_SOFTKEYBOARD_KEY+0x49)
#define XUI_SOFTKEYBOARD_KEYJ (XUI_SOFTKEYBOARD_KEY+0x4a)
#define XUI_SOFTKEYBOARD_KEYK (XUI_SOFTKEYBOARD_KEY+0x4b)
#define XUI_SOFTKEYBOARD_KEYL (XUI_SOFTKEYBOARD_KEY+0x4c)
#define XUI_SOFTKEYBOARD_KEYM (XUI_SOFTKEYBOARD_KEY+0x4d)
#define XUI_SOFTKEYBOARD_KEYN (XUI_SOFTKEYBOARD_KEY+0x4e)
#define XUI_SOFTKEYBOARD_KEYO (XUI_SOFTKEYBOARD_KEY+0x4f)
#define XUI_SOFTKEYBOARD_KEYP (XUI_SOFTKEYBOARD_KEY+0x50)
#define XUI_SOFTKEYBOARD_KEYQ (XUI_SOFTKEYBOARD_KEY+0x51)
#define XUI_SOFTKEYBOARD_KEYR (XUI_SOFTKEYBOARD_KEY+0x52)
#define XUI_SOFTKEYBOARD_KEYS (XUI_SOFTKEYBOARD_KEY+0x53)
#define XUI_SOFTKEYBOARD_KEYT (XUI_SOFTKEYBOARD_KEY+0x54)
#define XUI_SOFTKEYBOARD_KEYU (XUI_SOFTKEYBOARD_KEY+0x55)
#define XUI_SOFTKEYBOARD_KEYV (XUI_SOFTKEYBOARD_KEY+0x56)
#define XUI_SOFTKEYBOARD_KEYW (XUI_SOFTKEYBOARD_KEY+0x57)
#define XUI_SOFTKEYBOARD_KEYX (XUI_SOFTKEYBOARD_KEY+0x58)
#define XUI_SOFTKEYBOARD_KEYY (XUI_SOFTKEYBOARD_KEY+0x59)
#define XUI_SOFTKEYBOARD_KEYZ (XUI_SOFTKEYBOARD_KEY+0x5a)

#define XUI_SOFTKEYBOARD_KEYBRACKETLEFT (XUI_SOFTKEYBOARD_KEY+0x5b)
#define XUI_SOFTKEYBOARD_KEYBACKSLASH (XUI_SOFTKEYBOARD_KEY+0x5c)
#define XUI_SOFTKEYBOARD_KEYBRACKETRIGHT (XUI_SOFTKEYBOARD_KEY+0x5d)
#define XUI_SOFTKEYBOARD_KEYCARET (XUI_SOFTKEYBOARD_KEY+0x5e)
#define XUI_SOFTKEYBOARD_KEYUNDERSCORE (XUI_SOFTKEYBOARD_KEY+0x5f)
#define XUI_SOFTKEYBOARD_KEYBACKQUOTE (XUI_SOFTKEYBOARD_KEY+0x60)

#define XUI_SOFTKEYBOARD_KEYa (XUI_SOFTKEYBOARD_KEY+0x61)
#define XUI_SOFTKEYBOARD_KEYb (XUI_SOFTKEYBOARD_KEY+0x62)
#define XUI_SOFTKEYBOARD_KEYc (XUI_SOFTKEYBOARD_KEY+0x63)
#define XUI_SOFTKEYBOARD_KEYd (XUI_SOFTKEYBOARD_KEY+0x64)
#define XUI_SOFTKEYBOARD_KEYe (XUI_SOFTKEYBOARD_KEY+0x65)
#define XUI_SOFTKEYBOARD_KEYf (XUI_SOFTKEYBOARD_KEY+0x66)
#define XUI_SOFTKEYBOARD_KEYg (XUI_SOFTKEYBOARD_KEY+0x67)
#define XUI_SOFTKEYBOARD_KEYh (XUI_SOFTKEYBOARD_KEY+0x68)
#define XUI_SOFTKEYBOARD_KEYi (XUI_SOFTKEYBOARD_KEY+0x69)
#define XUI_SOFTKEYBOARD_KEYj (XUI_SOFTKEYBOARD_KEY+0x6a)
#define XUI_SOFTKEYBOARD_KEYk (XUI_SOFTKEYBOARD_KEY+0x6b)
#define XUI_SOFTKEYBOARD_KEYl (XUI_SOFTKEYBOARD_KEY+0x6c)
#define XUI_SOFTKEYBOARD_KEYm (XUI_SOFTKEYBOARD_KEY+0x6d)
#define XUI_SOFTKEYBOARD_KEYn (XUI_SOFTKEYBOARD_KEY+0x6e)
#define XUI_SOFTKEYBOARD_KEYo (XUI_SOFTKEYBOARD_KEY+0x6f)
#define XUI_SOFTKEYBOARD_KEYp (XUI_SOFTKEYBOARD_KEY+0x70)
#define XUI_SOFTKEYBOARD_KEYq (XUI_SOFTKEYBOARD_KEY+0x71)
#define XUI_SOFTKEYBOARD_KEYr (XUI_SOFTKEYBOARD_KEY+0x72)
#define XUI_SOFTKEYBOARD_KEYs (XUI_SOFTKEYBOARD_KEY+0x73)
#define XUI_SOFTKEYBOARD_KEYt (XUI_SOFTKEYBOARD_KEY+0x74)
#define XUI_SOFTKEYBOARD_KEYu (XUI_SOFTKEYBOARD_KEY+0x75)
#define XUI_SOFTKEYBOARD_KEYv (XUI_SOFTKEYBOARD_KEY+0x76)
#define XUI_SOFTKEYBOARD_KEYw (XUI_SOFTKEYBOARD_KEY+0x77)
#define XUI_SOFTKEYBOARD_KEYx (XUI_SOFTKEYBOARD_KEY+0x78)
#define XUI_SOFTKEYBOARD_KEYy (XUI_SOFTKEYBOARD_KEY+0x79)
#define XUI_SOFTKEYBOARD_KEYz (XUI_SOFTKEYBOARD_KEY+0x7a)

#define XUI_SOFTKEYBOARD_KEYBRACELEFT (XUI_SOFTKEYBOARD_KEY+0x7b)
#define XUI_SOFTKEYBOARD_KEYBAR (XUI_SOFTKEYBOARD_KEY+0x7c)
#define XUI_SOFTKEYBOARD_KEYBRACERIGHT (XUI_SOFTKEYBOARD_KEY+0x7d)
#define XUI_SOFTKEYBOARD_KEYTILDE (XUI_SOFTKEYBOARD_KEY+0x7e)

/*--------------------------------------------
 * Structures
 *-------------------------------------------*/
typedef struct {
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
} XuiColor;
 
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
	void *priv;
} XuiImg;

typedef struct {
	int btn_round;
	XuiColor btn_bg;
	char *text;
	XuiColor text_fg;
	XuiFont  *text_font;
	int text_x;
	int text_y;
	int text_height;
	XuiImg *img;
	int img_x;
	int img_y;
	XuiBgStyle img_style;
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
	XuiFont *font;
	int size;
	XuiColor fg;
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
int XuiCanvasDrawText(XuiWindow *window, unsigned int x, unsigned int y, unsigned int height,XuiFont *font, XuiTextStyle textstyle, XuiColor fg, const char *text);
int XuiCanvasDrawTextEx(XuiWindow *window, unsigned int x, unsigned int y, unsigned int height, XuiFont *font, XuiTextStyle textstyle, XuiColor fg, char *text, int linebreak);
int XuiCanvasDrawImg(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, XuiBgStyle bgstyle, XuiImg *img);
int XuiCanvasDrawRect(XuiWindow *window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, XuiColor fg, int round, int fill);
void XuiCanvasMoveToY(XuiWindow *window, unsigned int my);
void XuiCanvasSetBackground(XuiWindow *window, XuiBgStyle bgstyle, XuiImg *img, XuiColor bg);
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
