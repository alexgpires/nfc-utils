#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <nfc/nfc.h>
#include <freefare.h>

/*
    Stupid program in order to test "Mifare UID Authentication"... 
    This program just test if the card has the authorized UID.
    
    This should not be real ! But it is... 
    Lots of physical access control use this simple test... 

    Arod
    12/2014
*/

// gcc mifare_access_control.c -o mifare_access_control -lnfc -lusb -lfreefare

int main (int argc, const char *argv[]){

    char *AUTHORIZED_KEY = "b6566eea";

    nfc_context *context;
    nfc_device *pnd;
    MifareTag *tags = NULL;
    int i;
    char *uid;

    nfc_init(&context);
    if(context == NULL){
        printf("Unable to init libnfc\n");
        exit(EXIT_FAILURE);
    }

    pnd = nfc_open(context, NULL);
    if (pnd == NULL){
        printf("ERROR: Unable to open NFC device.\n");
        exit(EXIT_FAILURE);
    }
    printf("NFC reader: %s opened\n", nfc_device_get_name(pnd));


    while(1) {
        tags = freefare_get_tags(pnd);
        if (!tags) {
            printf("no tag found\n");
        }
        else {
            for (i=0; tags[i]; i++){
                // type Mifare 1k (S50) ?
                if (freefare_get_tag_type(tags[i]) == CLASSIC_1K){
                    // get uid of card
                    uid = freefare_get_tag_uid(tags[i]);
                    printf("UID is %s -> ", uid);
                    if (strcmp(uid, AUTHORIZED_KEY) == 0) {
                        printf("OK !! Access granted !\n");
                    }
                    else {
                        printf("Access denied !\n");
                    }
                }
            }
            freefare_free_tags(tags);
        }
        sleep(1);
    }

    // not used :)
    nfc_close(pnd);
    nfc_exit(context);
    exit(EXIT_SUCCESS);
}
