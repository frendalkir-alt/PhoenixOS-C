#define _CRT_SECURE_NO_WARNINGS
#include "../../include/phoenixos.h"
#include <io.h>

void cmd_tree(const char* path) {
    AddOutputLine("Directory listing:");
    struct _finddata_t fileinfo;
    intptr_t handle = _findfirst("*.*", &fileinfo);
    if (handle != -1) {
        do {
            if (strcmp(fileinfo.name, ".") && strcmp(fileinfo.name, "..")) {
                char line[300];
                if (fileinfo.attrib & _A_SUBDIR)
                    sprintf(line, "<DIR> %s", fileinfo.name);
                else
                    sprintf(line, "      %s", fileinfo.name);
                AddOutputLine(line);
            }
        } while (_findnext(handle, &fileinfo) == 0);
        _findclose(handle);
    }
}