


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
    addToDictionary(dict, "movs", 2, "00100", 3, 8, 0, 0, 1);
    addToDictionary(dict, "ands", 2, "0100000000", 3, 3, 0, 1, 0);
    addToDictionary(dict,"eors",2,"0100000001",3,3,0,1, 0);
    addToDictionary(dict,"lsls",2,"0100000010",3,3,0,1,0);
    addToDictionary(dict,"lsrs",2,"0100000011",3,3,0,1, 0);
}

void addToInstructionDictionary(struct InstructionDictionary *dict, const char *key, int argCount, const char *arg1, const char *arg2, const char *arg3) {
    if (dict->size < MAX_DICTIONARY_SIZE) {
        strcpy(dict->pairs[dict->size].key, key);
        dict->pairs[dict->size].argCount = argCount;
        strncpy(dict->pairs[dict->size].args[0], arg1, MAX_ARG_LENGTH - 1);
        dict->pairs[dict->size].args[0][MAX_ARG_LENGTH - 1] = '\0';
        strncpy(dict->pairs[dict->size].args[1], arg2, MAX_ARG_LENGTH - 1);
        dict->pairs[dict->size].args[1][MAX_ARG_LENGTH - 1] = '\0';
        strncpy(dict->pairs[dict->size].args[2], arg3, MAX_ARG_LENGTH - 1);
        dict->pairs[dict->size].args[2][MAX_ARG_LENGTH - 1] = '\0';
        dict->size++;
    } else {
        printf("InstructionDictionary is full, cannot add more elements.\n");
    }
}


int getValueFromInstructionDictionary(struct InstructionDictionary *dict, const char *key, int *argCount, char args[MAX_ARGS][MAX_ARG_LENGTH]) {
    for (int i = 0; i < dict->size; i++) {
        if (strcmp(dict->pairs[i].key, key) == 0) {
            *argCount = dict->pairs[i].argCount;
            for (int j = 0; j < *argCount; j++) {
                strncpy(args[j], dict->pairs[i].args[j], MAX_ARG_LENGTH);
            }
            return 1; // Key found
        }
    }
    return 0; // Key not found
}

void printInstructionDictionary(struct InstructionDictionary *dict) {
    for (int i = 0; i < dict->size; i++) {
        printf("Key: %s, Arg Count: %d, Args: ", dict->pairs[i].key, dict->pairs[i].argCount);
        for (int j = 0; j < dict->pairs[i].argCount; j++) {
            printf("%s ", dict->pairs[i].args[j]);
        }
        printf("\n");
    }
}

void addToDictionary(struct Dictionary *dict, const char *key, int argCount, const char *initbinary, int arg1, int arg2, int arg3, int reversed, int isImm) {
    if (dict->size < MAX_DICTIONARY_SIZE) {
        strcpy(dict->pairs[dict->size].key, key);
        dict->pairs[dict->size].argCount = argCount;
        strncpy(dict->pairs[dict->size].initbinary, initbinary, MAX_INIT_BIN);
        dict->pairs[dict->size].initbinary[MAX_INIT_BIN] = '\0';  // Ensure null-termination
        dict->pairs[dict->size].reversed=reversed;
        dict->pairs[dict->size].args[0] = arg1;
        dict->pairs[dict->size].args[1] = arg2;
        dict->pairs[dict->size].args[2] = arg3;
        dict->pairs[dict->size].isImm=isImm;

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


int getValueFromDictionary(struct Dictionary *dict, char *key, int isImm, int *argCount, int args[MAX_ARGS], const char *initbinary, int *reversed) {
    for (int i = 0; i < dict->size; i++) {
        if (strcmp(dict->pairs[i].key, key) == 0 && isImm==dict->pairs[i].isImm) {
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
