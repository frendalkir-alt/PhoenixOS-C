#include "../../include/phoenixos.h"
#define _CRT_SECURE_NO_WARNINGS

void cmd_cd(const char* path) {
    if (path[0] == '\0') {
        char currentDir[MAX_PATH];
        if (_getcwd(currentDir, MAX_PATH)) {
            AddOutputLine(currentDir);
        }
        return;
    }

    if (_chdir(path) != 0) {
        char msg[300];
        sprintf(msg, "The system cannot find the path specified: %s", path);
        AddOutputLine(msg);
    }
}