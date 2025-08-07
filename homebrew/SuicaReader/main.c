#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "osal.h"
#include "xui.h"

void printHex(const unsigned char *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

XuiWindow *root;
XuiFont *font;

int _init() {
    int status;

     char *xui_argv[] = {"ROTATE=90", "STATUSBAR=32"};
    XuiOpen(sizeof(xui_argv) / sizeof(xui_argv[0]), xui_argv);

    root = XuiRootCanvas();
    font = XuiCreateFont("/usr/font/tm_font", 0, XUI_FONT_GREY);

    XuiWindow *statusbar;
    statusbar = XuiStatusbarCanvas();
    XuiCanvasSetBackground(statusbar, XUI_BG_NORMAL, 0, XuiColor(255, 255, 255, 255));
    XuiCanvasSetBackground(root, XUI_BG_NORMAL, 0, XuiColor(255, 255, 255, 255));

    XuiClearArea(root, 0, 0, 239, 288);
    XuiCanvasDrawText(root, (240 - XuiTextWidth(font, 25, "Waiting for card")) / 2, 140, 25, font, 0, XuiColor(255, 0, 0, 0), "Waiting for card");

    status = OsPiccOpen();
    if (status != 0) { printf("OsPiccOpen failed\n"); return -1; }

    status = OsPiccInitFelica(2, 1);
    if (status != 0) { printf("OsPiccInitFelica failed\n"); return -1; }

    unsigned char tx_poll[6] = {0x06, 0x00, 0xFF, 0xFF, 0x00, 0x00};
    unsigned char rx_poll[256] = {0};
    int rx_poll_len = sizeof(rx_poll);

    status = OsPiccTransfer(tx_poll, sizeof(tx_poll), rx_poll, &rx_poll_len);
    while (status != 0) {
        printf("Polling failed, retrying...\n");
        usleep(50000);
        rx_poll_len = sizeof(rx_poll);
        status = OsPiccTransfer(tx_poll, sizeof(tx_poll), rx_poll, &rx_poll_len);
    }


    printf("Polling Response:\n");
    printHex(rx_poll, rx_poll_len);

    if (rx_poll_len < 10 || rx_poll[1] != 0x01) {
        printf("Invalid Polling response\n");
        return -1;
    }

    unsigned char IDm[8];
    memcpy(IDm, &rx_poll[2], 8);
    printf("IDm: ");
    printHex(IDm, 8);

    // === 2) Request Response ===
    unsigned char tx_rr[10] = {0x0A, 0x04};
    memcpy(&tx_rr[2], IDm, 8);

    unsigned char rx_rr[256] = {0};
    int rx_rr_len = sizeof(rx_rr);

    status = OsPiccTransfer(tx_rr, sizeof(tx_rr), rx_rr, &rx_rr_len);
    printf("Request Response status: %d\n", status);
    printHex(rx_rr, rx_rr_len);

    // === 3) Read Without Encryption – Block 0 of Service 0x090F ===
    
    unsigned char tx_read[16] = {
        0x10, 0x06
    };
    memcpy(&tx_read[2], IDm, 8);
    tx_read[10] = 0x01;       // Number of Services
    tx_read[11] = 0x0F;       // Service Code list LSB
    tx_read[12] = 0x09;       // Service Code list MSB
    tx_read[13] = 0x01;       // Number of Blocks
    tx_read[14] = 0x80;       // Block List 2-byte format
    tx_read[15] = 0x00;       // Block 0

    unsigned char rx_read[256] = {0};
    int rx_read_len = sizeof(rx_read);

    status = OsPiccTransfer(tx_read, sizeof(tx_read), rx_read, &rx_read_len);
    printf("Read Without Encryption status: %d\n", status);
    printHex(rx_read, rx_read_len);

    if (rx_read_len > 13 && rx_read[1] == 0x07) {
        unsigned short balance = (rx_read[24] << 8) | rx_read[23];
        printf("Balance: %d JPY\n", balance);
        XuiClearArea(root, 0, 0, 239, 288);
        char balance_str[32];
        snprintf(balance_str, sizeof(balance_str), "Balance: %d JPY", balance);
        XuiCanvasDrawText(root, (240 - XuiTextWidth(font, 25, balance_str)) / 2, 140, 25, font, 0, XuiColor(255, 0, 0, 0), balance_str);
        OsPiccRemove();
        OsPiccClose();
        OsBeep(4, 1000);
        sleep(5);

    } else {
        printf("Failed to read balance block\n");
        OsPiccClose();
    }



    return 0;
}
