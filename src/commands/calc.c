#define _CRT_SECURE_NO_WARNINGS
#include "../../include/phoenixos.h"
#include <stdlib.h>
#include <math.h>

void cmd_calc(const char* expr) {
    if (!expr || strlen(expr) == 0) {
        AddOutputLine("Usage: calc <expression>");
        AddOutputLine("Example: calc 2+2, calc 10*5, calc sqrt(16)");
        return;
    }

    // Простой парсер для выражений типа "2+2"
    double result = 0;
    char op = '\0';
    int i = 0;

    while (expr[i] && expr[i] != '+' && expr[i] != '-' && expr[i] != '*' && expr[i] != '/') {
        i++;
    }

    if (expr[i]) {
        op = expr[i];
        char left[100], right[100];
        strncpy(left, expr, i);
        left[i] = '\0';
        strcpy(right, expr + i + 1);

        double a = atof(left);
        double b = atof(right);

        switch(op) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/': 
                if (b != 0) result = a / b;
                else { AddOutputLine("Error: Division by zero"); return; }
                break;
            default: AddOutputLine("Error: Unsupported operator"); return;
        }

        char msg[100];
        sprintf(msg, "Result: %.2f", result);
        AddOutputLine(msg);
    } else {
        AddOutputLine("Error: Expression must contain operator (+, -, *, /)");
    }
}