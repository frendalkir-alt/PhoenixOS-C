#define _CRT_SECURE_NO_WARNINGS
#include "../../include/phoenixos.h"

void cmd_edit(const char* filename) {
    if (!filename || strlen(filename) == 0) {
        AddOutputLine("Usage: edit <file.txt>");
        return;
    }
    LoadFile(filename);
    g_inEditor = true;
}