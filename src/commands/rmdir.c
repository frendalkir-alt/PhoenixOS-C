#define _CRT_SECURE_NO_WARNINGS
#include "../../include/phoenixos.h"

void cmd_rmdir(const char* dirname) {
    if (!dirname || strlen(dirname) == 0) {
        AddOutputLine("Usage: rmdir <directory>");
        return;
    }

    if (_rmdir(dirname) == 0) {
        AddOutputLine("Directory removed.");
    } else {
        AddOutputLine("Error: Cannot remove directory");
    }
}