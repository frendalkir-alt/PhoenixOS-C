#define _CRT_SECURE_NO_WARNINGS
#include "../../include/phoenixos.h"

void cmd_copy(const char* args) {
    if (!args || strlen(args) == 0) {
        AddOutputLine("Usage: copy <source> <destination>");
        return;
    }

    char source[256] = {0};
    char dest[256] = {0};
    int i = 0;
    while (args[i] && args[i] != ' ') i++;
    if (args[i]) {
        strncpy(source, args, i);
        source[i] = '\0';
        strcpy(dest, args + i + 1);
    } else {
        AddOutputLine("Usage: copy <source> <destination>");
        return;
    }

    if (CopyFile(source, dest, FALSE)) {
        AddOutputLine("File copied successfully.");
    } else {
        AddOutputLine("Error: Cannot copy file");
    }
}