/**
 * @file osal.h
 * @brief Header for Prolin API programming.
 *
 * This file was manually assembled based on the Prolin API Programming Guide V2.5.3. It is incomplete and may contain errors.
 * Authors:
 * - Milan Babuljak (hiimmilan.dev), 2024
 * - Richard Gráčik (Morc), 2024
 */


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

// RF
#define PCD_ERR_PAR_FLAG         -2901  // Parity error.
#define PCD_ERR_CRC_FLAG         -2902  // CRC error.
#define PCD_ERR_WTO_FLAG         -2903  // Timeout or no card.
#define PCD_ERR_COLL_FLAG        -2904  // Multiple cards collision.
#define PCD_ERR_ECD_FLAG         -2905  // Frame format error.
#define PCD_ERR_EMD_FLAG         -2906  // Interference.
#define PCD_ERR_COM_FLAG         -2907  // Chip error, fail to communicate correctly.
#define PCD_ERR_AUT_FLAG         -2908  // M1 authentication error.
#define PCD_ERR_TRANSMIT_FLAG    -2909  // Transmission error.
#define PCD_ERR_PROTOCOL_FLAG    -2910  // Protocol error.
#define PCD_ERR_PARAMFILE_FLAG   -2911  // Configuration file does not exist.
#define PCD_ERR_USER_CANCEL      -2912  // Transaction is cancelled.
#define PCD_ERR_CARRIER_OBTAIN_FLAG -2913 // No obtained carrier.
#define PCD_ERR_CONFIG_FLAG      -2914  // Fail to configure register.
#define PCD_ERR_RXLEN_EXCEED_FLAG -2915 // The returned data length exceeds the set receiving length.
#define PCD_ERR_NOT_ALLOWED_FLAG -2951 // Parameter error or invalid value.
#define PCD_CHIP_ABNORMAL        -2952  // Chip is abnormal or does not exist.
#define PCD_CHIP_NOT_OPENED      -2953  // Module is not open.
#define PCD_CHIP_CARDEXIST       -2954  // Card is not removed.
#define PCD_ERR_NOT_IDLE_FLAG    -2955  // Card is not in idle state.
#define PCD_ERR_NOT_POLLING_FLAG -2956  // No polling.
#define PCD_ERR_NOT_WAKEUP_FLAG  -2957  // Card does not wakeup.
#define PCD_ERR_NOT_ACTIVE_FLAG  -2958  // Card is not activated.
#define PCD_ERR_NOT_SUPPORT      -2959  // Chip is unsupported.
#define ERR_BATTERY_VOLTAGE_TOO_LOW -1024 // Battery voltage is too low.

// Battery
#define POWER_USB 1
#define POWER_BATTERY 3


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

typedef struct {
    unsigned char TrackData[256]; /* Decoded bit stream*/
    int DataLen; /* Track data length*/
    int Status; /*Track data status, 0 means succeeded, other value means failed*/
} ST_MSR_DATA;

typedef struct pcd_user_t {
     unsigned char wait_retry_limit_w; /* S(WTX) responds to write permission of sending times*/ 
     unsigned int wait_retry_limit_val; /*S(WTX)responds to the maximum repetition times.*/
     unsigned char check_cancel_key_w; /*respond to write permission of the cancel key*/ 
     unsigned char check_cancel_key_val; /* 0: No response to the cancel key; 1: Respond to the cancel key */
     int (*check_cancel_key_function)(void); /*Check whether the cancel key is pressed. If set check_cancel_key_w=1 and check_cancel_key_val=1, check_cancel_key_function () will be called during RF card transaction process. If check_cancel_key_function () returns 0, it means the cancel key is not pressed. If the function returns non-zero, it means the cancel key is pressed and will exit transaction by force,*/ 
     unsigned char os_picc_transfer_set_w; /*1 means “os_picc_transfer_set_val” value is valid, 0 means “os_picc_transfer_set_val” value is invalid*/ 
     unsigned char os_picc_transfer_set_val; /* Set OsPiccTransfer receive/send: Bit0=0: send disable CRC; Bit0=1: send enable CRC; Bit1=0: receive disable CRC; Bit1=1: receive enable CRC*/ 
     unsigned char anti_interference_flag; /*Anti-interference setting when searching the card; 1- enable anti-interference, 0- disable an-interference.*/ 
     unsigned char protocol_layer_fwt_set_w; /*1 indicates that the value of protocol_layer_fwt_set_val is valid, 0 indicates that the value of protocol_layer_fwt_set_val is invalid */
    unsigned int protocol_layer_fwt_set_val; /* Set the frame wait time for the protocol layer (value of FWT)*/ 
    unsigned char os_picc_transfer_rxlen_set_w; /*1 indicates that the value of os_picc_transfer_rxlen_set_val is vaild, 0 indicates that the value ofos_picc_transfer_rxlen_set_val is invalid */ 
    unsigned int os_picc_transfer_rxlen_set_val; /*Set the maximum allowable data length in the half-duplex block (OsPiccTransfer) transfer*/ 
    unsigned char os_picc_transfer_direct_transmit_set_w; /* 1 means the data is transmitted in the way of data stream, that is, the half-duplex block transmission protocol is not applicable;0 represents data transfer using the half duplex block transfer protocol */ 
    unsigned char configure_technology_type_w; /* Set whether the configure_technology_type_val parameter can be written：1—allowed, other values--not allowed */ 
    unsigned char configure_technology_type_val; /* Configure the modulation technology type of the data sent by OsPiccTransfer()*/ 
    unsigned char reserved[34]; /* Reserved byte, for future use. sizeof(PCD_USER_ST)= 76*/ 
} PCD_USER_ST;

typedef struct { 
    unsigned char Cmd[4]; /*CLA, INS, P1, P2*/ 
    int LC; /* The valid length of DataIn sent to IC card */ 
    unsigned char DataIn[512];/* The data sent to ICC */ 
    int LE; /* The expected length of returned data*/ 
} ST_APDU_REQ;

typedef struct { 
    int LenOut; /* Data length returned from ICC*/ 
    unsigned char DataOut[512]; /*Data pointer returned from ICC */ 
    unsigned char SWA; /*status word 1 of ICC */ 
    unsigned char SWB; /* status word 2 of ICC */ 
} ST_APDU_RSP;

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
int OsPrnOpen(unsigned int printertype, const char* targetname );
void OsPrnSelectFontSize(int SingleCodeWidth, int SingleCodeHeight, int MultiCodeWidth, int MultiCodeHeight);
int OsPrnSetDirection(int direction);
int OsPrnPrintText(const char *text);
void OsPrnSetGray(int Level);
void OsPrnClose(void);

/*--------------------------------------------
 * Function Prototypes - MSR (Magnetic Stripe Reader)
 *-------------------------------------------*/
int OsMsrOpen(void);
int OsMsrClose(void);
void OsMsrReset(void);
int OsMsrSwiped(void);
int OsMsrRead(ST_MSR_DATA *Track1, ST_MSR_DATA *Track2, ST_MSR_DATA *Track3);

/*--------------------------------------------
 * Function Prototypes - Network Communication
 *-------------------------------------------*/
int OsNetAddArp(const char *ip, const char *mac);
int OsNetPing(const char *ip);
int OsNetSetConfig(const char *config);

/*--------------------------------------------
 * Function Prototypes - File System
 *-------------------------------------------*/
int OsMount(const char *Source, const char *Target, const char *FileSystemType, unsigned long MountFlags, const void *Data);
int OsUmount(const char *Target, int Flags);

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
int OsPiccResetCarrier(void);
int OsPiccPoll( char *pcPiccType, unsigned char *pucATQx );
int OsPiccAntiSel( const char pcPiccType, unsigned char *pucUID, const unsigned char ucATQ0, unsigned char *pucSAK );
int OsPiccActive( const char pcPiccType, unsigned char *pucRATS );
int OsPiccTransfer(const unsigned char*pucTxBuff, int iTxLen, unsigned char* pucRxBuff, int *piRxLen );
int OsPiccRemove(void );
int OsMifareAuthority(unsigned char *uid, unsigned char blk_no, unsigned chargroup, unsigned char *psw);
int OsMifareOperate(unsigned charucOpCode, unsigned charucSrcBlkNo, unsigned char*pucVal, unsigned char ucDesBlkNo );
int OsPiccInitFelica(unsigned char ucSpeed, unsigned char ucModInvert );
int OsPiccIsoCommand(int cid, ST_APDU_REQ *ApduReq, ST_APDU_RSP *ApduRsp);
int OsPiccSetUserConfig(PCD_USER_ST *pcd_user_config) ;
int OsPiccGetUserConfig(PCD_USER_ST *pcd_user_config);
int OsPiccApplePoll(char *pcPiccType, unsigned char *pucATQx, unsigned char *pucSendData);
int OsPiccInitIso15693(unsigned char ucDataCodeMode);
int OsPiccOffCarrier(void);
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
int OsCheckPowerSupply(void);
int OsSysSleep();
int OsSysSleepEx(int level);
int OsPowerOff(void);

#endif // PROLIN_API_H
