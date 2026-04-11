#ifndef PHOENIXOS_H
#define PHOENIXOS_H

// Поддержка bool в чистом C
#ifndef __cplusplus
typedef int bool;
#define true 1
#define false 0
#endif

// Стандартные заголовки Windows
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <io.h>
#include <shellapi.h>

// === Глобальные переменные ===

// Основные (терминал)
extern HWND g_hwnd;
extern HFONT g_hFont;
extern char g_inputBuffer[256];
extern int g_inputPos;
extern char g_outputLines[500][256];
extern int g_outputCount;
extern bool g_welcomeShown;

// Редактор
extern bool g_inEditor;
extern int g_editorScroll;
extern int g_editorLineCount;
extern int g_editorCursorY;

// === Объявления функций ===

// Оконная процедура
LRESULT CALLBACK ShellProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// GUI
void InitializeWelcome();

// Shell
void AddOutputLine(const char* line);
void ClearOutput();
void ExecuteCommand(const char* cmd);

// Команды системы
void cmd_dir();
void cmd_cd(const char* path);
void cmd_cls();
void cmd_help();
void cmd_exit();
void cmd_brows(const char* url);
void cmd_playmp3(const char* filename);
void cmd_stop();
void cmd_echo(const char* text);
void cmd_ver();
void cmd_sysinfo();
void cmd_tree(const char* path);
void cmd_copy(const char* args);
void cmd_delete(const char* filename);
void cmd_edit(const char* filename);
void cmd_mkdir(const char* dirname);
void cmd_mklink(const char* args);
void cmd_rmdir(const char* dirname);
void cmd_color(const char* args);
void cmd_time();
void cmd_date();
void cmd_reboot();
void cmd_calc(const char* expr);
void cmd_debug();

// Встроенный редактор
void LoadFile(const char* filename);
void SaveFile();
void DrawEditor(HDC hdc);
void EditorHandleChar(WPARAM wParam);

#endif // PHOENIXOS_H