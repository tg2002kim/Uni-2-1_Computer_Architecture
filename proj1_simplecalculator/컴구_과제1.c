/*
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable:4996)
#define MAX_REGISTERS 10

// Function to calculate Greatest Common Divisor (GCD)
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int main(int argc, char* argv[]) {
    FILE* stream;
    char* line = NULL;
    size_t len = 0;  // ssize_t 대신 size_t를 사용
    int nread;
    char op;
    char operand_type1, operand_type2; // Operand types: '0' for intermediate value, 'R' for register
    int operand1, operand2; // Operands
    int registers[MAX_REGISTERS] = { 0 }; // Initialize all registers to 0

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    stream = fopen(argv[1], "r");
    if (stream == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Read a line from input.txt file
    while ((nread = getline(&line, &len, stream)) != -1) {
        // Parse input line
        sscanf(line, "%c %c %d %c %d", &op, &operand_type1, &operand1, &operand_type2, &operand2);

        // Perform operations based on operator
        switch (op) {
        case '+':
            // Addition operation
            if (operand_type1 == 'R' && operand_type2 == 'R')
                registers[0] = registers[operand1] + registers[operand2];
            else if (operand_type1 == '0' && operand_type2 == '0')
                registers[0] = operand1 + operand2;
            else
                printf("Error: Invalid operands for addition!\n");
            break;
        case '-':
            // Subtraction operation
            if (operand_type1 == 'R' && operand_type2 == 'R')
                registers[0] = registers[operand1] - registers[operand2];
            else if (operand_type1 == '0' && operand_type2 == '0')
                registers[0] = operand1 - operand2;
            else
                printf("Error: Invalid operands for subtraction!\n");
            break;
        case '*':
            // Multiplication operation
            if (operand_type1 == 'R' && operand_type2 == 'R')
                registers[0] = registers[operand1] * registers[operand2];
            else if (operand_type1 == '0' && operand_type2 == '0')
                registers[0] = operand1 * operand2;
            else
                printf("Error: Invalid operands for multiplication!\n");
            break;
        case '/':
            // Division operation
            if (operand_type1 == 'R' && operand_type2 == 'R') {
                if (registers[operand2] != 0)
                    registers[0] = registers[operand1] / registers[operand2];
                else
                    printf("Error: Division by zero!\n");
            }
            else if (operand_type1 == '0' && operand_type2 == '0') {
                if (operand2 != 0)
                    registers[0] = operand1 / operand2;
                else
                    printf("Error: Division by zero!\n");
            }
            else {
                printf("Error: Invalid operands for division!\n");
            }
            break;
        case 'M':
            // Move operation
            if (operand_type1 == '0' && operand_type2 == 'R')
                registers[operand2] = operand1;
            else if (operand_type1 == 'R' && operand_type2 == 'R')
                registers[operand2] = registers[operand1];
            else
                printf("Error: Invalid operands for move operation!\n");
            break;
        case 'C':
            // Comparison operation
            if (registers[operand1] == registers[operand2])
                registers[0] = 0;
            else if (registers[operand1] > registers[operand2])
                registers[0] = 1;
            else
                registers[0] = -1;
            break;
        case 'J':
            // Jump operation
            fseek(stream, (operand1 - 1) * (nread + 1), SEEK_SET);
            break;
        case 'B':
            // Conditional Jump operation (BEQ)
            if (registers[0] == 0)
                fseek(stream, (operand1 - 1) * (nread + 1), SEEK_SET);
            break;
        case 'G':
            // GCD calculation operation
            registers[0] = gcd(registers[operand1], registers[operand2]);
            break;
        case 'H':
            printf("Halting execution.\n");
            fclose(stream);
            if (line) free(line);
            exit(EXIT_SUCCESS);
        default:
            printf("Error: Invalid opcode!\n");
        }

        // Print result and register state
        printf("Opcode: %c Operand1: %d Operand2: %d\n", op, operand1, operand2);
        printf("Registers: ");
        for (int i = 0; i < MAX_REGISTERS; ++i) {
            printf("R%d: %d ", i, registers[i]);
        }
        printf("\n");
    }

    free(line);
    fclose(stream);

    return 0;
}
*/