#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include <ctype.h>


int containsHash(char *str) {
    return (strchr(str, '#') != NULL);
}

char* signedIntTo8bitBinary(int num) {
    // Ensure num fits within 8 bits
    num &= 0xFF;

    // Allocate memory for the binary string (including room for null terminator)
    char* binaryString = (char*)malloc(9);

    if (binaryString == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Loop through each bit and build the binary string
    for (int i = 7; i >= 0; i--) {
        binaryString[7 - i] = ((num & (1 << i)) ? '1' : '0');
    }

    // Null-terminate the string
    binaryString[8] = '\0';

    return binaryString;
}
char* binary_to_hex(const char* binary) {
    // Validate the input binary string
    for (const char* c = binary; *c != '\0'; ++c) {
        if (*c != '0' && *c != '1') {
            return "Error: Input is not a binary string";
        }
    }

    // Calculate the length of the output hex string
    size_t len = (strlen(binary) + 3) / 4; // (binary length + 3) / 4

    // Allocate memory for the hex string
    char* hex = (char*)malloc((len + 1) * sizeof(char)); // +1 for the null terminator

    // Convert binary to hex
    for (size_t i = 0; i < len; ++i) {
        int value = 0;
        for (size_t j = 0; j < 4; ++j) {
            value = (value << 1) + (*binary++ - '0');
        }
        hex[i] = (value < 10) ? (char)(value + '0') : (char)(value - 10 + 'A');
    }

    hex[len] = '\0'; // Null terminate the hex string
    return hex;
}

char *decToBinaryWithSize(int n, int size) {
    if (size < 0) {
        fprintf(stderr, "Invalid size argument\n");
        exit(EXIT_FAILURE);
    }

    char *binaryResult = (char *)malloc((size + 1) * sizeof(char));
    if (binaryResult == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the binary string with '0'
    for (int i = 0; i < size; i++) {
        binaryResult[i] = '0';
    }
    binaryResult[size] = '\0';

    int i = size - 1;
    while (n > 0 && i >= 0) {
        binaryResult[i] = (n % 2) + '0';
        n = n / 2;
        i--;
    }

    return binaryResult;
}

void parse_instruction(const char* instruction, char* command, char* arg1, char* arg2, char* arg3, int* isImm, int* argCount) {
    // Use sscanf to extract components from the instruction
    // sscanf(instruction, "%s %s %s %s", command, arg1, arg2, arg3);
    *argCount = 0;
    int arg2Pos = 0;
    int arg3Pos = 0;

    while (*instruction && *instruction != '\n') {
        if (*instruction == ' ') {
            if (*(instruction + 1) != '\0' && *(instruction + 1) != ' ') {
                (*argCount)++;
            }
            instruction++;
        } else if (*instruction == ',') {
            instruction++;
            continue;
        }

        if (*instruction != '\0') {
            switch (*argCount) {
                case 0:
                    *command = *instruction;
                    command++;
                    break;
                case 1:
                    *arg1 = *instruction;
                    arg1++;
                    break;
                case 2:
                    *(arg2 + arg2Pos) = *instruction;
                    arg2Pos++;
                    break;
                case 3:
                    *(arg3 + arg3Pos) = *instruction;
                    arg3Pos++;
                    break;
            }
        }
        instruction++;
    }

    *command = '\0';
    *arg1 = '\0';
    *(arg2 + arg2Pos) = '\0';
    *(arg3 + arg3Pos) = '\0';
    *isImm = 0;

    if ((containsHash(arg1)) || (containsHash(arg2)) || (containsHash(arg3))) {
        // Tous les imm ont un # dedans, donc si on a un #, on est dans un imm
        *isImm = 1;
    }
}

int getConditionCode(char *command) {
    char *conditionList[] = {"EQ", "NE", "CS", "MI", "PL", "VS", "VC", "HI", "LS", "GE", "LT", "GT", "LE", "AL", "NV"};
    for (int i = 0; i < 15; i++) {
            if (strcmp(command + 1, conditionList[i]) == 0) {
                return i + 1;
        }
    }
    return -1;  // Indicates invalid or not found
}


int extract_numbers(const char* str) {
    int result = 0;
    int negative = 1;
    while (*str) {
        if (isdigit(*str)) {
            result = result * 10 + (*str - '0');
        }
        if (*str == '-'){
            negative = -1;
        }
        str++;
    }


    return result*negative;
}
void removeNonNumericInPlace(char *input) {
    if (input == NULL) {
        fprintf(stderr, "Input string is NULL\n");
        return;
    }

    int outputIndex = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        if (isdigit(input[i])) {
            input[outputIndex++] = input[i];
        }
    }

    // Null-terminate the modified string
    input[outputIndex] = '\0';
}

void processInstruction(const char *instruction, struct Dictionary *dict,char* reponse) {
    char initbinary[MAX_INIT_BIN + 1];
    char command[50];
    char arg1[50];
    char arg2[50];
    char arg3[50];
    int reversed;
    int isImm;
    int argCount;
    // Parse the instruction
    parse_instruction(instruction, command, arg1, arg2, arg3, &isImm,&argCount);

    printf("full text: %s\n", instruction);
    printf("command: %s\n", command);
    printf("arg1: %s\n", arg1);
    printf("arg2: %s\n", arg2);
    printf("arg3: %s\n", arg3);
    printf("isImm : %d\n", isImm);
    // Check if the command is in the dictionaries

    int args[MAX_ARGS]={0};
    int found = 0;
    // Check in the instruction dictionary
    found = getValueFromDictionary(dict, command, isImm, argCount, args, initbinary,&reversed);
    removeNonNumericInPlace(initbinary);

    // Process the instruction based on the dictionary entry

    if ((strcmp(command,"str") == 0)||((strcmp(command,"ldr") == 0))){ //Cas a part
        if (strcmp(command,"str")==0){strcpy(initbinary,"10010");}
        else{strcpy(initbinary,"10011");}
        argCount = 2;
        reversed = 0;
        found = 1;
        args[0] = 3;
        args[1] = 8;
    }
    if ((strlen(command) == 3)&&(command[0] == 'b')){ //Cas a part
        strcpy(initbinary,"1101");
        char *totalargs = (char *)malloc(strlen(initbinary) + 1);
        strcpy(totalargs, initbinary);
        totalargs = (char *)realloc(totalargs, strlen(totalargs) + strlen(decToBinaryWithSize(getConditionCode(command),4)) + 1);
        strcat(totalargs,decToBinaryWithSize(getConditionCode(command),4));
        totalargs = (char *)realloc(totalargs, strlen(totalargs) + strlen(signedIntTo8bitBinary(extract_numbers(arg1))) + 1);
        strcat(totalargs,signedIntTo8bitBinary(extract_numbers(arg1)));
        strcpy(reponse,binary_to_hex(totalargs));
        return;
    }
/*
    if (found) {
        printf("IS FOUND !!!\n");
        printf("Command: %s\n", command);
        printf("Argument Count: %d\n", argCount);
        printf("Init Binary: %s\n", initbinary);
        printf("Arguments: ");
        for (int i = 0; i < argCount; i++) {
            printf("%d ", args[i]);
        }
        printf("\n");
    } else {
        printf("Command not found in dictionaries.\n");
    }
*/
    int arg1Numb = extract_numbers(arg1);
    int arg2Numb = extract_numbers(arg2);
    int arg3Numb = extract_numbers(arg3);
    if ((strcmp(command,"add") == 0)|| (strcmp(command,"sub") == 0)|| (strcmp(command,"str") == 0)|| (strcmp(command,"ldr") == 0)){
        arg2Numb = arg2Numb /4;
    }
   // printf("arg2 %d\n",arg2Numb);
    char *binaryArg1 = decToBinaryWithSize(arg1Numb,args[0]);
    //printf("%s\n",binaryArg1);
    char *binaryArg2 = decToBinaryWithSize(arg2Numb,args[1]);
    //printf("%s\n",binaryArg2);
    char *binaryArg3 = decToBinaryWithSize(arg3Numb,args[2]);
    //printf("%s\n",binaryArg3);
    char *totalargs = (char *)malloc(strlen(initbinary) + strlen(binaryArg1) + 1);
    strcpy(totalargs, initbinary);
    //printf("test %d\n", argCount);
    if (reversed){
        if (argCount==3) {
            char *copie=binaryArg1;
            binaryArg1=binaryArg3;
            binaryArg3=copie;
        }
        else if(argCount==2) {
            char *copie=binaryArg1;
            binaryArg1=binaryArg2;
            binaryArg2=copie;
        }
    }
    printf("1 %s\n", binaryArg1);
    strcat(totalargs, binaryArg1);
    totalargs = (char *)realloc(totalargs, strlen(totalargs) + strlen(binaryArg2) + 1);
    //printf("2 %s\n", totalargs);
    strcat(totalargs, binaryArg2);
    totalargs = (char *)realloc(totalargs, strlen(totalargs) + strlen(binaryArg3) + 1);
    strcat(totalargs, binaryArg3);

    removeNonNumericInPlace(totalargs);
    printf("%s ",binary_to_hex(totalargs));
    strcpy(reponse,binary_to_hex(totalargs));
}

/*
int main() {
    struct Dictionary myDict;
    initDictionary(&myDict);
    char answer[16];
    const char *instruction1 = "ldr r2,[sp, #4]";
    processInstruction(instruction1, &myDict,&answer);

    printf("%s",answer);
    return 0;
}

*/
#define MAX_LINE_LENGTH 1000

int main() {
    printf("bon début");
    FILE *file;
    char line[MAX_LINE_LENGTH];
    struct Dictionary myDict;
    initDictionary(&myDict);
    FILE *fichier = fopen("instructions.bin", "w");
    // Open the file for reading
    file = fopen("instructions.s", "r");  // Replace "example.txt" with your file name
    if (file == NULL) {
        fprintf(stderr, "Error opening the file\n");
        exit(EXIT_FAILURE);
    }

    // Read the file line by line and call processInstruction() for each line
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        char answer[16];
        // Call the function to process the instruction
        processInstruction(line,&myDict,answer);
        fprintf(fichier, "%s", answer);

    }

    // Close the file
    fclose(file);

    // Écrit la chaîne dans le fichier


    // Ferme le fichier
    fclose(fichier);

    return 0; // Fin du programme sans erreur
}

