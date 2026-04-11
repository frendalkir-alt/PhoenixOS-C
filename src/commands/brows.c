#define _CRT_SECURE_NO_WARNINGS
#include "../../include/phoenixos.h"
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

void cmd_brows(const char* url) {
    if (!url || strlen(url) == 0) {
        AddOutputLine("Usage: brows <https://example.com>");
        return;
    }

    char fullUrl[512];
    if (strncmp(url, "http://", 7) != 0 && strncmp(url, "https://", 8) != 0) {
        sprintf(fullUrl, "https://%s", url);
    } else {
        strcpy(fullUrl, url);
    }

    HINTERNET hOpen = InternetOpen(
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36",
        INTERNET_OPEN_TYPE_DIRECT,
        NULL, NULL, 0
    );

    if (!hOpen) {
        AddOutputLine("Error: Cannot initialize internet");
        return;
    }

    DWORD flags = SECURITY_FLAG_IGNORE_CERT_CN_INVALID |
                  SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
                  SECURITY_FLAG_IGNORE_UNKNOWN_CA;
    InternetSetOption(hOpen, INTERNET_OPTION_SECURITY_FLAGS, &flags, sizeof(flags));

    HINTERNET hFile = InternetOpenUrl(hOpen, fullUrl, NULL, 0, 
        INTERNET_FLAG_RELOAD | 
        INTERNET_FLAG_SECURE | 
        INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
        INTERNET_FLAG_IGNORE_CERT_DATE_INVALID,
        0
    );

    if (!hFile) {
        DWORD err = GetLastError();
        char msg[100];
        sprintf(msg, "Error %lu: Page not accessible", err);
        AddOutputLine(msg);
        InternetCloseHandle(hOpen);
        return;
    }

    AddOutputLine("Loading text content...");

    char buffer[2048];
    DWORD bytesRead;
    int lines = 0;

    while (InternetReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0 && lines < 40) {
        buffer[bytesRead] = '\0';

        char* in = buffer;
        char clean[2048] = {0};
        int j = 0;
        bool inTag = false;

        for (int i = 0; in[i] && j < 2047; i++) {
            if (in[i] == '<') {
                inTag = true;
            } else if (in[i] == '>') {
                inTag = false;
                clean[j++] = ' ';
            } else if (!inTag) {
                if (in[i] >= 32 && in[i] <= 126) {
                    clean[j++] = in[i];
                } else if (in[i] == '\n' || in[i] == '\r') {
                    clean[j++] = ' ';
                }
            }
        }
        clean[j] = '\0';

        char* token = strtok(clean, " ");
        char line[256] = {0};
        int len = 0;

        while (token && lines < 40) {
            if (len + strlen(token) + 1 < 250) {
                if (len > 0) strcat(line, " ");
                strcat(line, token);
                len += strlen(token) + 1;
            } else {
                if (strlen(line) > 10) {
                    AddOutputLine(line);
                    lines++;
                }
                strcpy(line, token);
                len = strlen(token);
            }
            token = strtok(NULL, " ");
        }

        if (strlen(line) > 10 && lines < 40) {
            AddOutputLine(line);
            lines++;
        }
    }

    InternetCloseHandle(hFile);
    InternetCloseHandle(hOpen);

    if (lines == 0) {
        AddOutputLine("No readable content found");
    }
}