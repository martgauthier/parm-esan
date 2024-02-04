


#include <stdio.h>
#include <string.h>
#include "dictionary.h"

#define MAX_DICTIONARY_SIZE 40
#define MAX_ARGS 3
#define MAX_ARG_LENGTH 50
#define MAX_INIT_BIN 11

void initDictionary(struct Dictionary *dict) {
    dict->size = 1;
    // Add some initial key-value pairs during initialization
    addToDictionary(dict, "movs", 2, "00100", 3, 8, 0, 0);
    addToDictionary(dict, "ands", 2, "0100000000", 3, 3, 0, 1);
    addToDictionary(dict,"eors",2,"0100000001",3,3,0,1);
    addToDictionary(dict,"lsls",2,"0100000010",3,3,0,1);
    addToDictionary(dict,"lsrs",2,"0100000011",3,3,0,1);
    addToDictionary(dict,"rsbs",2,"0100001001",3,3,0,1);
    addToDictionary(dict,"asrs",2,"0100000100",3,3,0,1); //reg
    addToDictionary(dict,"adcs",2,"0100000101",3,3,0,1);
    addToDictionary(dict,"rors",2,"0100000111",3,3,0,1);
    addToDictionary(dict,"sbcs",2,"0100000110",3,3,0,1);
    addToDictionary(dict,"tst",2,"0100001000",3,3,0,1);
    addToDictionary(dict,"cmn",2,"0100001011",3,3,0,1);
    addToDictionary(dict,"cmp",2,"0100001010",3,3,0,1); //reg
    addToDictionary(dict,"orrs",2,"0100001100",3,3,0,1);
    addToDictionary(dict,"muls",2,"0100001101",3,3,0,1);
    addToDictionary(dict,"bics",2,"0100001110",3,3,0,1);
    addToDictionary(dict,"mvns",2,"0100001111",3,3,0,1);


}


void addToDictionary(struct Dictionary *dict, const char *key, int argCount, const char *initbinary, int arg1, int arg2, int arg3, int reversed) {
    if (dict->size < MAX_DICTIONARY_SIZE) {
        strcpy(dict->pairs[dict->size].key, key);
        dict->pairs[dict->size].argCount = argCount;
        strncpy(dict->pairs[dict->size].initbinary, initbinary, MAX_INIT_BIN);
        dict->pairs[dict->size].initbinary[MAX_INIT_BIN] = '\0';  // Ensure null-termination
        dict->pairs[dict->size].reversed=reversed;
        dict->pairs[dict->size].args[0] = arg1;
        dict->pairs[dict->size].args[1] = arg2;
        dict->pairs[dict->size].args[2] = arg3;

        if(reversed) {
            if(argCount==3) {
                dict->pairs[dict->size].args[0]=arg3;
                dict->pairs[dict->size].args[2]=arg1;
            }
            else if(argCount==2) {
                dict->pairs[dict->size].args[0]=arg2;
                dict->pairs[dict->size].args[1]=arg1;
            }
        }

        dict->size++;
    } else {
        printf("Dictionary is full, cannot add more elements.\n");
    }
}


int getValueFromDictionary(struct Dictionary *dict, char *key, int *argCount, int args[MAX_ARGS], const char *initbinary, int *reversed) {
    for (int i = 0; i < dict->size; i++) {
        if (strcmp(dict->pairs[i].key, key) == 0) {
            *argCount = dict->pairs[i].argCount;
            strncpy(initbinary, dict->pairs[i].initbinary, MAX_INIT_BIN);
            *reversed = dict->pairs[i].reversed;
            for (int j = 0; j < *argCount; j++) {
                args[j] = dict->pairs[i].args[j];
            }
            return 1; // Key found
        }
    }
    return 0; // Key not found
}

void printDictionary(struct Dictionary *dict) {
    for (int i = 0; i < dict->size; i++) {
        printf("Key: %s, Arg Count: %d, Init Binary: %s, Args: ", dict->pairs[i].key, dict->pairs[i].argCount, dict->pairs[i].initbinary);
        for (int j = 0; j < dict->pairs[i].argCount; j++) {
            printf("%d ", dict->pairs[i].args[j]);
        }
        printf("\n");
    }
}
