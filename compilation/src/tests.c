#include <stdio.h>

void parse_instruction(const char* instruction, char* command, char* arg1, char* arg2, char* arg3) {
    // Use sscanf to extract components from the instruction
    sscanf(instruction, "%s %s %s %s", command, arg1, arg2, arg3);
}

int main() {
    // Example usage
    const char* instruction = "movs r1 #170";
    char command[50];
    char arg1[50];
    char arg2[50];
    char arg3[50];

    // Parse the instruction
    parse_instruction(instruction, command, arg1, arg2, arg3);

    // Display the parsed components
    printf("Command: %s\n", command);
    printf("Arg1: %s\n", arg1);
    printf("Arg2: %s\n", arg2);
    printf("Arg3: %s\n", arg3);

    return 0;
}
