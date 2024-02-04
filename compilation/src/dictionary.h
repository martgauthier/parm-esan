
// dictionary.h

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DICTIONARY_SIZE 40
#define MAX_ARGS 3
#define MAX_ARG_LENGTH 50
#define MAX_INIT_BIN 20

struct KeyInstructionValuePair {
    char key[50];
    int argCount;
    char args[MAX_ARGS][MAX_ARG_LENGTH];
};

struct InstructionDictionary {
    struct KeyInstructionValuePair pairs[MAX_DICTIONARY_SIZE];
    int size;
};

struct KeyValuePair {
    char key[50];
    int argCount;
    char initbinary[MAX_INIT_BIN + 1];  // +1 for null-termination
    int args[MAX_ARGS];
    int reversed;//0 if normal, 1 if reversed
    int isImm;//0 if register,  1 if imm
    //int needsDivision; //0 if normal, 1 if STR,LDR...
};

struct Dictionary {
    struct KeyValuePair pairs[MAX_DICTIONARY_SIZE];
    int size;
};
void addToDictionary(struct Dictionary *dict, const char *key, int argCount, const char *initbinary, int arg1, int arg2, int arg3, int reversed, int isImm);

void initDictionary(struct Dictionary *dict);

int getValueFromDictionary(struct Dictionary *dict, char *key, int isImm, int argCount, int args[MAX_ARGS], const char *inibinary,int *reversed);

void printDictionary(struct Dictionary *dict);

#endif
