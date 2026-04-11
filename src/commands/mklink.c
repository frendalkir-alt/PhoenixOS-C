#define _CRT_SECURE_NO_WARNINGS
#include "../../include/phoenixos.h"

void cmd_mklink(const char* args) {
    AddOutputLine("Symbolic links are not supported in this version.");
    AddOutputLine("Use 'copy' or 'mkdir' instead.");
}