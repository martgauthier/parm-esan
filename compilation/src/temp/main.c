

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include <ctype.h>

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

void parse_instruction(const char* instruction, char* command, char* arg1, char* arg2, char* arg3) {
    // Use sscanf to extract components from the instruction
    //sscanf(instruction, "%s %s %s %s", command, arg1, arg2, arg3);
    int argCount=0;
    while(*(instruction) && *instruction!='\n') {
        if (*instruction == ' ') {
            argCount++;
            instruction++;
        }
        else if(*instruction==',') {
            instruction++;
            continue;
        }

        if (*instruction != '\0') {
            switch (argCount) {
                case 0:
                    *command = *instruction;
                    command++;
                    break;
                case 1:
                    *arg1 = *instruction;
                    arg1++;
                    break;
                case 2:
                    *arg2 = *instruction;
                    arg2++;
                    break;
                case 3:
                    *arg3 = *instruction;
                    arg3++;
                    break;
            }
        }
        instruction++;
    }

    *command = '\0';
    *arg1 = '\0';
    *arg2 = '\0';
    *arg3='\0';
}

int extract_numbers(const char* str) {
    int result = 0;

    while (*str) {
        if (isdigit(*str)) {
            result = result * 10 + (*str - '0');
        }
        str++;
    }

    return result;
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

void processInstruction(const char *instruction, struct Dictionary *dict) {
    char initbinary[MAX_INIT_BIN + 1];
    char command[50];
    char arg1[50];
    char arg2[50];
    char arg3[50];
    int reversed;
    // Parse the instruction
    parse_instruction(instruction, command, arg1, arg2, arg3);
/*
    printf("full text: %s\n", instruction);
    printf("command: %s\n", command);
    printf("arg1: %s\n", arg1);
    printf("arg2: %s\n", arg2);
    printf("arg3: %s\n", arg3);
*/
    // Check if the command is in the dictionaries
    int argCount;
    int args[MAX_ARGS];
    int found = 0;
    // Check in the instruction dictionary
    found = getValueFromDictionary(dict, command, &argCount, args, initbinary,&reversed);
    if (!found){
        return;
    }
    removeNonNumericInPlace(initbinary);

    // Process the instruction based on the dictionary entry
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

    char *binaryArg1 = decToBinaryWithSize(extract_numbers(arg1),args[0]);
    //printf("%s\n",binaryArg1);
    char *binaryArg2 = decToBinaryWithSize(extract_numbers(arg2),args[1]);
    //printf("%s\n",binaryArg2);
    char *binaryArg3 = decToBinaryWithSize(extract_numbers(arg3),args[2]);
    //printf("%s\n",binaryArg3);
    char *totalargs = (char *)malloc(strlen(initbinary) + strlen(binaryArg1) + 1);
    strcpy(totalargs, initbinary);
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
    strcat(totalargs, binaryArg1);
    totalargs = (char *)realloc(totalargs, strlen(totalargs) + strlen(binaryArg2) + 1);
    strcat(totalargs, binaryArg2);
    //printf(totalargs);
    if(argCount == 3){
        totalargs = (char *)realloc(totalargs, strlen(totalargs) + strlen(binaryArg3) + 1);
        strcat(totalargs, binaryArg3);
    }


    removeNonNumericInPlace(totalargs);
    printf(" %s ",binary_to_hex(totalargs));
}

/*
int main() {
    struct Dictionary myDict;
    initDictionary(&myDict);

    const char *instruction1 = "movs r4, #161";
    processInstruction(instruction1, &myDict);

    return 0;
}

*/
#define MAX_LINE_LENGTH 1000

int main() {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    struct Dictionary myDict;
    initDictionary(&myDict);

    // Open the file for reading
    file = fopen("1-4_instructions.s", "r");  // Replace "example.txt" with your file name
    if (file == NULL) {
        fprintf(stderr, "Error opening the file\n");
        exit(EXIT_FAILURE);
    }

    // Read the file line by line and call processInstruction() for each line
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        // Call the function to process the instruction
        processInstruction(line,&myDict);
    }

    // Close the file
    fclose(file);

    return 0;
}

