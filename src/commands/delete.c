#define _CRT_SECURE_NO_WARNINGS
#include "../../include/phoenixos.h"

void cmd_delete(const char* filename) {
    if (!filename || strlen(filename) == 0) {
        AddOutputLine("Usage: delete <file>");
        return;
    }

    if (DeleteFile(filename)) {
        AddOutputLine("File deleted.");
    } else {
        AddOutputLine("Error: Cannot delete file");
    }
}