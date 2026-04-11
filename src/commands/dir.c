#include "../../include/phoenixos.h"
#define _CRT_SECURE_NO_WARNINGS

void cmd_dir() {
    struct _finddata_t f;
    intptr_t h = _findfirst("*.*", &f);
    if (h != -1) {
        do {
            if (strcmp(f.name, ".") && strcmp(f.name, "..")) {
                char line[300];
                if (f.attrib & _A_SUBDIR) 
                    sprintf(line, "<DIR> %s", f.name);
                else 
                    sprintf(line, "      %s", f.name);
                AddOutputLine(line);
            }
        } while (_findnext(h, &f) == 0);
        _findclose(h);
    }
}