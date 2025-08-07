#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "osal.h"

void printHex(const unsigned char *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int _init() {
    int status;

    // Open PICC
    status = OsPiccOpen();
    if (status != 0) { printf("OsPiccOpen failed\n"); return -1; }

    // Enable Felica mode
    status = OsPiccInitFelica(2, 1);
    if (status != 0) { printf("OsPiccInitFelica failed\n"); return -1; }

    // FeliCa Polling 
    unsigned char tx_poll[6] = {0x06, 0x00, 0xFF, 0xFF, 0x00, 0x00};
    unsigned char rx_poll[256] = {0};
    int rx_poll_len = sizeof(rx_poll);

    status = OsPiccTransfer(tx_poll, sizeof(tx_poll), rx_poll, &rx_poll_len);
    if (status != 0) { printf("Polling failed\n"); return -1; }

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

    // request Response ===
    unsigned char tx_rr[10] = {0x0A, 0x04};
    memcpy(&tx_rr[2], IDm, 8);

    unsigned char rx_rr[256] = {0};
    int rx_rr_len = sizeof(rx_rr);

    status = OsPiccTransfer(tx_rr, sizeof(tx_rr), rx_rr, &rx_rr_len);
    printf("Request Response status: %d\n", status);
    printHex(rx_rr, rx_rr_len);

    // read Without Encryption - block 0 of service 0x090F
    
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
    printf("RWE Status: %d\n", status);
    printHex(rx_read, rx_read_len);

    OsPiccRemove();
    OsPiccClose();
    exit(0);
}
