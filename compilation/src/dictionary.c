


#include <stdio.h>
#include <string.h>
#include "dictionary.h"

#define MAX_DICTIONARY_SIZE 40
#define MAX_ARGS 3
#define MAX_ARG_LENGTH 50
#define MAX_INIT_BIN 20

void initDictionary(struct Dictionary *dict) {
    dict->size = 1;
    addToDictionary(dict, "movs", 2, "00100", 3, 8, 0, 0,1); //imm
    addToDictionary(dict, "ands", 2, "0100000000", 3, 3, 0, 1,0);
    addToDictionary(dict,"eors",2,"0100000001",3,3,0,1,0);
    addToDictionary(dict,"lsls",2,"0100000010",3,3,0,1,0); //reg
    addToDictionary(dict,"lsls",3,"00000",5,3,3,1,1); //imm
    addToDictionary(dict,"lsrs",2,"0100000011",3,3,0,1,0); //reg
    addToDictionary(dict,"lsrs",3,"00001",5,3,3,1,1); //imm
    addToDictionary(dict,"rsbs",3,"0100001001",0,3,3,1,1); //imm
    addToDictionary(dict,"asrs",2,"0100000100",3,3,0,1,0); //reg
    addToDictionary(dict,"asrs",3,"00010",5,3,3,1,1); //imm
    addToDictionary(dict,"adcs",2,"0100000101",3,3,0,1,0);
    addToDictionary(dict,"rors",2,"0100000111",3,3,0,1,0);
    addToDictionary(dict,"sbcs",2,"0100000110",3,3,0,1,0);
    addToDictionary(dict,"tst",2,"0100001000",3,3,0,1,0);
    addToDictionary(dict,"cmn",2,"0100001011",3,3,0,1,0);
    addToDictionary(dict,"orrs",2,"0100001100",3,3,0,1,0);
    addToDictionary(dict,"muls",2,"0100001101",3,3,0,1,0);
    addToDictionary(dict,"bics",2,"0100001110",3,3,0,1,0);
    addToDictionary(dict,"mvns",2,"0100001111",3,3,0,1,0);

    addToDictionary(dict,"bics",2,"0100001110",3,3,0,1,0);

    addToDictionary(dict,"subs",3,"0001101",3,3,3,1,0); //reg
    addToDictionary(dict,"subs",3,"0001111",3,3,3,1,1); //imm 3
    addToDictionary(dict,"subs",2,"00111",3,8,0,0,1); //imm 8

    addToDictionary(dict,"adds",3,"0001100",3,3,3,1,0); //reg
    addToDictionary(dict,"adds",3,"0001110",3,3,3,1,1); //imm 3
    addToDictionary(dict,"adds",2,"00110",3,8,0,0,1); //imm 8

    addToDictionary(dict,"cmp",2,"0100001010",3,3,0,1,0); //reg
    addToDictionary(dict,"cmp",2,"00101",3,8,0,0,1); //imm

    addToDictionary(dict,"b",1,"11100",11,0,0,0,0); //Unconditionnal branching

    addToDictionary(dict,"add",2,"101100000",0,7,0,0,1); //imm
    addToDictionary(dict,"sub",2,"101100001",0,7,0,0,1); //imm



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


int getValueFromDictionary(struct Dictionary *dict, char *key, int isImm, int argCount, int args[MAX_ARGS], const char *initbinary, int *reversed) {
    for (int i = 0; i < dict->size; i++) {
        if (((strcmp(dict->pairs[i].key, key) == 0) && (dict->pairs[i].isImm == isImm))&&(dict->pairs[i].argCount == argCount)) {
            strncpy(initbinary, dict->pairs[i].initbinary, MAX_INIT_BIN);
            *reversed = dict->pairs[i].reversed;
            for (int j = 0; j < argCount; j++) {
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
