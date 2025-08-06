
#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

int regs[10] = { 0 };

// 덧셈
void add(int op1, int op2) {
    regs[0] = op1 + op2;
    printf("R0: %d = %d + %d\n", regs[0], op1, op2);
}

// 뺄셈
void sub(int op1, int op2) {
    regs[0] = op1 - op2;
    printf("R0: %d = %d - %d\n", regs[0], op1, op2);
}

// 곱셈
void mul(int op1, int op2) {
    regs[0] = op1 * op2;
    printf("R0: %d = %d * %d\n", regs[0], op1, op2);
}

// 나눗셈
void divide(int op1, int op2) {
    if (op2 == 0) {
        printf("오류: 0으로 나눌 수 없음\n");
        return;
    }
    regs[0] = op1 / op2;
    printf("R0: %d = %d / %d\n", regs[0], op1, op2);
}

// M 연산
void M(int op1, int op2) {
    regs[op2] = op1;
    printf("R%d: %d\n", op2, op1);
}
/*
// C 연산 (Compare)
void C(int op1, int op2) {
    if (op1 == op2) {
        regs[0] = 0;
    }
    else if (op1 > op2) {
        regs[0] = 1;
    }
    else {
        regs[0] = -1;
    }
}
*/

void C(char* op1_str, char* op2_str) {
    int res;
    int op1 = strtol(op1_str, NULL, 16);
    int op2 = strtol(op2_str, NULL, 16);

    if (op1 == op2)
        res = 0;
    else if (op1 > op2)
        res = 1;
    else
        res = -1;

    regs[0] = res;
    printf("R0: %d\n", res);
}


// J 연산 (Jump)
void J(int targetInstruction) {
    printf("Jump to instruction %d\n", targetInstruction);
    
}

// BEQ 연산 (Branch_Equal)
void B(int targetInstruction) {
    if (regs[0] == 0) {
        printf("Branch to instruction %d\n", targetInstruction);
        
    }
}

// GCD 연산 (Greatest common divisor)
void G(int a, int b) {
    int i, gcd;
    int s = (a < b) ? a : b;
    for (i = 1; i <= s; i++) {
        if ((a % i == 0) && (b % i == 0)) {
            gcd = i;
        }
    }
    regs[0] = gcd;
    printf("R0: %d \n", regs[0]);
}


int main() {
    FILE* fp;
    char* inst_reg = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));

    fp = fopen("input.txt", "r");
    if (fp == NULL) {
        perror("오류: input.txt파일이 존재하지 않음. \n");
        exit(EXIT_FAILURE);
    }

    if (inst_reg == NULL) {
        perror("오류: 메모리 할당 실패. \n");
        exit(EXIT_FAILURE);
    }

    while (fgets(inst_reg, MAX_LINE_LENGTH, fp) != NULL) {
        char* op = strtok(inst_reg, " ");
        char* op1_str = strtok(NULL, " ");
        char* op2_str = strtok(NULL, " \n");

        int op1, op2;

        if (op1_str[0] == 'R') {
            op1 = regs[op1_str[1] - '0'];
        }
        else {
            op1 = strtol(op1_str, NULL, 16);
        }

        if (op2_str[0] == 'R') {
            op2 = regs[op2_str[1] - '0'];
        }
        else {
            op2 = strtol(op2_str, NULL, 16);
        }

        if (strcmp(op, "+") == 0) {
            add(op1, op2);
        }
        else if (strcmp(op, "-") == 0) {
            sub(op1, op2);
        }
        else if (strcmp(op, "*") == 0) {
            mul(op1, op2);
        }
        else if (strcmp(op, "/") == 0) {
            divide(op1, op2);
        }
        else if (strcmp(op, "M") == 0) {
            M(op1, op2);
        }
        else if (strcmp(op, "C") == 0) {
            C(op1_str, op2_str);
        }
        else if (strcmp(op, "J") == 0) {
            J(op2);
        }
        else if (strcmp(op, "BEQ") == 0) {
            B(op2);
        }
        else if (strcmp(op, "GCD") == 0) {
            G(op1, op2);
        }
    }

    fclose(fp);
    free(inst_reg);

    return 0;
}
