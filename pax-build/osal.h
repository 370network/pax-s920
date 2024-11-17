#ifndef PROLIN_API_H
#define PROLIN_API_H

#include <stdint.h>

/*--------------------------------------------
 * Error Codes
 *-------------------------------------------*/
#define RET_OK                    0
#define ERR_INVALID_HANDLE       -1000
#define ERR_TIME_OUT             -1001
#define ERR_APP_NOT_EXIST        -1002
#define ERR_INVALID_PARAM        -1003
#define ERR_DEV_NOT_EXIST        -1004
#define ERR_DEV_BUSY             -1005
#define ERR_DEV_NOT_OPEN         -1006
#define ERR_ACCESS_DENY          -1007
#define ERR_FILE_NOT_FOUND       -2201
#define ERR_NO_SPACE             -2205
#define ERR_NEED_ADMIN           -2207

/*--------------------------------------------
 * File Types
 *-------------------------------------------*/
#define FILE_TYPE_APP            1
#define FILE_TYPE_APP_PARAM      2
#define FILE_TYPE_SYS_LIB        3
#define FILE_TYPE_PUB_KEY        4
#define FILE_TYPE_AUP            5

/*--------------------------------------------
 * Structures
 *-------------------------------------------*/
typedef struct {
    int Year;
    int Month;
    int Day;
    int Hour;
    int Minute;
    int Second;
    int DayOfWeek;
} ST_TIME;

typedef struct {
    unsigned long Start;
    unsigned long Stop;
    unsigned long TimeLeft;
} ST_TIMER;

typedef struct {
    char Id[32];
    char Name[64];
    char Bin[64];
    char Artwork[64];
    char Desc[64];
    char Vender[32];
    char Version[32];
} ST_APP_INFO;

typedef struct {
    unsigned char Left;
    unsigned char Top;
    unsigned char Width;
    unsigned char Height;
    unsigned char Dot[3072];
} FT_DOT;

typedef struct {
    char FileName[64];
    char FontName[64];
} FT_FONT;

/*--------------------------------------------
 * Function Prototypes - System
 *-------------------------------------------*/
int OsSetTime(const ST_TIME *Time);
void OsGetTime(ST_TIME *Time);
int OsTimerSet(ST_TIMER *Timer, unsigned long Ms);
int OsTimerCheck(ST_TIMER *Timer);
int OsInstallFile(const char *Name, const char *FileName, int FileType);
int OsUninstallFile(const char *AppName, int FileType);
int OsFirmwareGetVersion(char *Version, int Size);
int OsFirmwareUpgrade(const char *FwFileName);
void OsSaveCrashReport(int sig);

/*--------------------------------------------
 * Function Prototypes - PED
 *-------------------------------------------*/
int OsPedOpen(void);
int OsPedGetVer(char *Version, int Size);
int OsPedVerifyPlainPin(void);
int OsPedEraseKeys(void);
int OsPedSetFunctionKey(int KeyFlag);
void OsPedClose(void);
int OsPedWriteKey(int KeyType, const char *KeyData, int KeyLen);

/*--------------------------------------------
 * Function Prototypes - LCD
 *-------------------------------------------*/
int OsScrContrast(int contrast);
int OsScrBrightness(int brightness);
int OsScrGetSize(int *width, int *height);

/*--------------------------------------------
 * Function Prototypes - Printer
 *-------------------------------------------*/
int OsPrnOpen(void);
int OsPrnSetSize(int size);
int OsPrnSetDirection(int direction);
int OsPrnPrintText(const char *text);
void OsPrnClose(void);

/*--------------------------------------------
 * Function Prototypes - MSR (Magnetic Stripe Reader)
 *-------------------------------------------*/
int OsMsrOpen(void);
int OsMsrClose(void);
int OsMsrRead(char *track1, char *track2, char *track3);

/*--------------------------------------------
 * Function Prototypes - Network Communication
 *-------------------------------------------*/
int OsNetAddArp(const char *ip, const char *mac);
int OsNetPing(const char *ip);
int OsNetSetConfig(const char *config);

/*--------------------------------------------
 * Function Prototypes - File System
 *-------------------------------------------*/
int OsMount(const char *device, const char *mountPoint);
int OsUmount(const char *mountPoint);

/*--------------------------------------------
 * Function Prototypes - Encryption/Decryption
 *-------------------------------------------*/
int OsGetRandom(char *buffer, int size);
int OsSHA(const char *data, int dataLen, char *hash);
int OsDES(const char *data, char *encryptedData);
int OsAES(const char *data, char *encryptedData);
int OsRSA(const char *data, char *encryptedData);
int OsRSAKeyGen(char *publicKey, char *privateKey, int keySize);

/*--------------------------------------------
 * Function Prototypes - RF Reader
 *-------------------------------------------*/
int OsPiccOpen(void);
int OsPiccClose(void);
int OsPiccTransfer(const unsigned char *sendData, int sendLen, unsigned char *recvData, int *recvLen);
int OsPiccRemove(void);

/*--------------------------------------------
 * Function Prototypes - Wi-Fi
 *-------------------------------------------*/
int OsWifiOpen(void);
int OsWifiClose(void);
int OsWifiScan(void);
int OsWifiConnect(const char *ssid, const char *password);
int OsWifiDisconnect(void);

/*--------------------------------------------
 * Function Prototypes - Battery Management
 *-------------------------------------------*/
int OsCheckBattery(int *level);
int OsPowerOff(void);

#endif // PROLIN_API_H
