#define _CRT_SECURE_NO_WARNINGS
#include "../../include/phoenixos.h"

void cmd_mkdir(const char* dirname) {
    if (!dirname || strlen(dirname) == 0) {
        AddOutputLine("Usage: mkdir <directory>");
        return;
    }

    if (_mkdir(dirname) == 0) {
        AddOutputLine("Directory created.");
    } else {
        AddOutputLine("Error: Cannot create directory");
    }
}