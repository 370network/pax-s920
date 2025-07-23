/**
 * @file emv_example.c
 * This is an example of using PAX OSAL API to interact with standard payment cards (EMV Contactless Book D V2.1). 
 * Specifically, it demonstrates how to communucate with VISA card, as that's what I had laying around.
 *
 * NOTE: I had some problems with VISA ELECTRON cards, as they don't seem to be standard compliant.
 *
 * Milan Babuljak <milan.babuljak@hiimmilan.dev>, 2025
 */


#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "osal.h"


// You can safely ignore this, it's just a helper function to print hex and ASCII data
void printHex(const char *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", (unsigned char)data[i]);
    }
    printf("\n");
}

void printAscii(const char *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (data[i] >= 0x20 && data[i] <= 0x7E) {
            printf("%c", data[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
}



int _init() {

    // Open the PICC (Proximity Integrated Circuit Card) interface
    int status = OsPiccOpen();
    printf("OsPiccOpen returned %d\n", status);
    if (status < 0) {
        exit(0);
    }

    char uid[255] = {0};                             
    char pucATQx[256] = {0};   
    char pucSAK[256] = {0};
    char cardType[1] = {0};
    char putRATS[256] = {0};

    // Continue polling until a card is detected
    // When the card is detected, cardType will be set to either A or B depending on the card type.
    // and pucATQx will contain Answer To reQuest response based on the card type.
    status = OsPiccPoll(cardType, pucATQx);
    while (status != 0) {
        status = OsPiccPoll(cardType, pucATQx);
    }

    printf("Got Card!\n");
    printf("Card Type: %c\n", cardType[0]);
    printf("ATQx: ");
    printHex(pucATQx, sizeof(pucATQx));
    printf("\n");


    // We now know that we have a card present in field, but we need to select it.
    // That's done by calling OsPiccAntiSel function.
    // This function will return SAK (Select Acknowledge) response which is used to
    // determine the UID of the card and some other parameters.
    status = OsPiccAntiSel('A', pucATQx, 0, pucSAK);  
    printf("OsPiccAntiSel returned %d\n", status);
    printf("SAK: ");
    printHex(pucSAK, sizeof(pucSAK));


    // We need to also activate the card to be able to communicate with it.
    // This is done by calling OsPiccActive function.
    // This function will return RAT (Request Acknowledge Type) response which is used to
    // determine the communication parameters of the card.
    status = OsPiccActive('A', putRATS);
    printf("OsPiccActive returned %d\n", status);
    printf("RATS: ");
    printHex(putRATS, sizeof(putRATS));


    // Now we can send APDU commands to the card.
    // For this example, I will send SELECT PSA - 2PAY.SYS.DDF01
    // APDUs are represented here with ST_APDU_REQ and ST_APDU_RSP structures.
    // I'd suggest looking in osal.h as well as Prolin API documentation for more details.
    ST_APDU_REQ apdu_req = {0};
    ST_APDU_RSP apdu_rsp = {0};

    apdu_req.Cmd[0] = 0x00; // CLA - Standard commands
    apdu_req.Cmd[1] = 0xA4; // INS - SELECT APPLICATION
    apdu_req.Cmd[2] = 0x04; // P1  
    apdu_req.Cmd[3] = 0x00; // P2

    apdu_req.LC = 14; // Length of data - selecting 2PAY.SYS.DDF01
    memcpy(apdu_req.DataIn, "2PAY.SYS.DDF01", 14);
    apdu_req.LE = 255;

    // ISO 7816-4 APDU command
    status = OsPiccIsoCommand(pucSAK, &apdu_req, &apdu_rsp);
    printf("OsPiccIsoCommand returned %d\n", status);

    // Now we can read the response from the card.
    printf("APDU HEX Response: ");
    printHex(apdu_rsp.DataOut, apdu_rsp.LenOut);
    printf("\n");

    printf("APDU Response ASCII: ");
    printAscii(apdu_rsp.DataOut, apdu_rsp.LenOut);


    // At this point, we have successfully communicated with the card and selected the application..
    // I'm gonna continue by selecting the VISA application.

    apdu_req.Cmd[0] = 0x00;
    apdu_req.Cmd[1] = 0xA4; 
    apdu_req.Cmd[2] = 0x04; 
    apdu_req.Cmd[3] = 0x00; 
    apdu_req.LC = 7; 
    memcpy(apdu_req.DataIn, "\xA0\x00\x00\x00\x03\x10\x10", 7); // 0xA0, 0x00, 0x00, 0x00, 0x03, 0x10, 0x10 is the AID for VISA
    apdu_req.LE = 255;

    status = OsPiccIsoCommand(pucSAK, &apdu_req, &apdu_rsp);
    printf("OsPiccIsoCommand returned %d\n", status);
    printf("APDU HEX Response: ");
    printHex(apdu_rsp.DataOut, apdu_rsp.LenOut);
    printf("\n");
    printf("APDU Response ASCII: ");
    printAscii(apdu_rsp.DataOut, apdu_rsp.LenOut);
    
    // This will properly remove the card in accordance with EMV specifications.
    OsPiccRemove();

    OsBeep(3, 500);
    exit(0);
}