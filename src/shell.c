#define _CRT_SECURE_NO_WARNINGS
#include "../include/phoenixos.h"
#include <string.h>
#include <stdlib.h>

// Внешние переменные (определены в main.c)
extern char g_outputLines[500][256];
extern int g_outputCount;

void AddOutputLine(const char* line) {
    if (g_outputCount >= 500) {
        // Сдвигаем всё вверх
        for (int i = 0; i < 499; i++) {
            strcpy(g_outputLines[i], g_outputLines[i + 1]);
        }
        g_outputCount = 499;
    }
    strncpy(g_outputLines[g_outputCount], line, 255);
    g_outputLines[g_outputCount][255] = '\0';
    g_outputCount++;
}

void ClearOutput() {
    g_outputCount = 0;
}

// Вспомогательная функция: разделить команду и аргументы
void ParseCommand(const char* input, char* command, char* args) {
    int i = 0;
    while (input[i] && input[i] != ' ' && input[i] != '\t') {
        command[i] = input[i];
        i++;
    }
    command[i] = '\0';

    while (input[i] == ' ' || input[i] == '\t') {
        i++;
    }

    strcpy(args, input + i);
}

void ExecuteCommand(const char* input) {
    if (!input || strlen(input) == 0) {
        return;
    }

    char command[256] = {0};
    char args[256] = {0};
    ParseCommand(input, command, args);

    if (strcmp(command, "dir") == 0) {
        cmd_dir();
    } else if (strcmp(command, "cd") == 0) {
        cmd_cd(args);
    } else if (strcmp(command, "cls") == 0) {
        cmd_cls();
    } else if (strcmp(command, "help") == 0) {
        cmd_help();
    } else if (strcmp(command, "exit") == 0) {
        PostQuitMessage(0);
    } else if (strcmp(command, "brows") == 0) {
        cmd_brows(args);
    } else if (strcmp(command, "playmp3") == 0) {
        cmd_playmp3(args);
    } else if (strcmp(command, "stop") == 0) {
        cmd_stop();
    } else if (strcmp(command, "echo") == 0) {
        cmd_echo(args);
    } else if (strcmp(command, "ver") == 0) {
        cmd_ver();
    } else if (strcmp(command, "sysinfo") == 0) {
        cmd_sysinfo();
    } else if (strcmp(command, "tree") == 0) {
        cmd_tree(args);
    } else if (strcmp(command, "copy") == 0) {
        cmd_copy(args);
    } else if (strcmp(command, "delete") == 0) {
        cmd_delete(args);
    } else if (strcmp(command, "edit") == 0) {
        cmd_edit(args);
    } else if (strcmp(command, "mkdir") == 0) {
        cmd_mkdir(args);
    } else if (strcmp(command, "mklink") == 0) {
        cmd_mklink(args);
    } else if (strcmp(command, "rmdir") == 0) {
        cmd_rmdir(args);
    } else if (strcmp(command, "color") == 0) {
        cmd_color(args);
    } else if (strcmp(command, "time") == 0) {
        cmd_time();
    } else if (strcmp(command, "date") == 0) {
        cmd_date();
    } else if (strcmp(command, "reboot") == 0) {
        cmd_reboot();
    } else if (strcmp(command, "calc") == 0) {
        cmd_calc(args);
    } else {
        char msg[300];
        sprintf(msg, "'%s' is not recognized as an internal or external command", command);
        AddOutputLine(msg);
    }
}