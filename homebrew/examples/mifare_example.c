/**
 * @file mifare_example.c
 * This is an example of using PAX OSAL API to interact with MiFare cards. 
 * This example is minimum code to demonstrate the basic operations e.g reading a first block of a MiFare card.
 *
 * Milan Babuljak <milan.babuljak@hiimmilan.dev>, 2025
 */


#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "osal.h"

// Different cards could have different parameters. This shuld work on MiFare Classic 1K.
// UltraLight have 7 byte UID

#define UID_SIZE 4
#define BLOCK_DATA_SIZE 16
#define PASSWORD_SIZE 6


int _init() {

    
    int status = OsPiccOpen();
    printf("OsPiccOpen returned %d\n", status);
    if (status < 0) {
        exit(0);
    }

    char uid[UID_SIZE] = {0};                             
    char pucATQx[256] = {0};   
    char pucSAK[256] = {0};
    char block_data[BLOCK_DATA_SIZE] = {0};
    char password[PASSWORD_SIZE] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    char cardType[1] = {0};

    status = OsPiccPoll(cardType, pucATQx);
    

    // Continue polling until a card is detected
    // When the card is detected, cardType will be set to either A or B depending on the card type.
    // and pucATQx will contain Answer To reQuest response based on the card type.
    // Response from MiFare 1K to ATQx is 0x00 0x04 0x08 0x00. 
    while (status != 0) {
        status = OsPiccPoll(cardType, pucATQx);
    }

    printf("Got Card!\n");


    // We now know that we have a card present in field, but we need to select it.
    // That's done by calling OsPiccAntiSel function.
    // This function will return SAK (Select Acknowledge) response which is used to
    // determine the UID of the card and some other parameters.
    status = OsPiccAntiSel('A', pucATQx, 0, pucSAK);  
    printf("OsPiccAntiSel returned %d\n", status);

    memcpy(uid, pucATQx, UID_SIZE);
    printf("UID: %02X %02X %02X %02X\n", uid[0], uid[1], uid[2], uid[3]);

    // If we have our card active, now we can perform an authorization operation.
    // Mifere cards have a concept of blocks, and each block can be protected by a password.
    // In this example, we will use a default password, with unlocking the first block of the card.
    status = OsMifareAuthority(uid, 0, 'A', password);
    printf("OsMifareAuthority returned %d\n", status);

    // After the authorization, we can finally read the first block of the card.
    status = OsMifareOperate('R', 0, block_data, 0);
    printf("OsMifareOperate returned %d\n", status);

    printf("Block data: ");
    for (int i = 0; i < BLOCK_DATA_SIZE; i++) {
        printf("%02X ", (unsigned char)block_data[i]);
    }
    printf("\n");

    OsBeep(3, 500);
    exit(0);
}