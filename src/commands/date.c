#define _CRT_SECURE_NO_WARNINGS
#include "../../include/phoenixos.h"
#include <time.h>

void cmd_date() {
    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    char buffer[100];
    strftime(buffer, sizeof(buffer), "Current date: %Y-%m-%d", local);
    AddOutputLine(buffer);
}