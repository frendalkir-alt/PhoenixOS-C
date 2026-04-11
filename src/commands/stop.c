#define _CRT_SECURE_NO_WARNINGS
#include "../../include/phoenixos.h"
#include <tlhelp32.h>
#include <psapi.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

void cmd_stop() {
    mciSendString("stop mymp3", NULL, 0, NULL);
    mciSendString("close mymp3", NULL, 0, NULL);
    AddOutputLine("MP3 playback stopped.");
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        AddOutputLine("Error: Cannot access process list");
        return;
    }

    PROCESSENTRY32 pe = {0};
    pe.dwSize = sizeof(PROCESSENTRY32);
    bool found = false;

    if (Process32First(hSnap, &pe)) {
        do {
            if (strstr(pe.szExeFile, "wmplayer.exe") ||
                strstr(pe.szExeFile, "Music.UI.exe") ||
                strstr(pe.szExeFile, "Groove.exe")) {
                HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                if (hProc) {
                    TerminateProcess(hProc, 0);
                    CloseHandle(hProc);
                    found = true;
                }
            }
        } while (Process32Next(hSnap, &pe));
    }

    CloseHandle(hSnap);

    if (found) {
        AddOutputLine("MP3 playback stopped.");
    } else {
        AddOutputLine("No active media player found.");
    }
}