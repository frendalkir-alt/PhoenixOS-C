#define _CRT_SECURE_NO_WARNINGS
#include "../../include/phoenixos.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

void cmd_playmp3(const char* filename) {
    if (!filename || strlen(filename) == 0) {
        AddOutputLine("Usage: playmp3 <file.mp3>");
        return;
    }

    if (_access(filename, 0) != 0) {
        char msg[300];
        sprintf(msg, "File not found: %s", filename);
        AddOutputLine(msg);
        return;
    }

    // Воспроизводим через MCI
    char command[512];
    sprintf(command, "open \"%s\" type mpegvideo alias mymp3", filename);
    mciSendString(command, NULL, 0, NULL);
    mciSendString("play mymp3", NULL, 0, NULL);

    char msg[300];
    sprintf(msg, "Playing: %s", filename);
    AddOutputLine(msg);
}