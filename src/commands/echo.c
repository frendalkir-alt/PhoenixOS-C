#define _CRT_SECURE_NO_WARNINGS
#include "../../include/phoenixos.h"

void cmd_echo(const char* text) {
    if (text && strlen(text) > 0) {
        AddOutputLine(text);
    } else {
        AddOutputLine("Usage: echo <text>");
    }
}