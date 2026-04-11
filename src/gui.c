#define _CRT_SECURE_NO_WARNINGS
#include "../include/phoenixos.h"
#include <direct.h>

// Все переменные объявлены в phoenixos.h
extern HWND g_hwnd;
extern HFONT g_hFont;
extern char g_inputBuffer[256];
extern int g_inputPos;
extern char g_outputLines[500][256];
extern int g_outputCount;
extern bool g_inEditor;
extern int g_editorScroll;
extern int g_editorLineCount;
extern int g_editorCursorY;

// Флаг показа приветствия
bool g_welcomeShown = false;

void InitializeWelcome() {
    if (!g_welcomeShown) {
        AddOutputLine("");
        AddOutputLine("  _____  _                      _       ____   _____ ");
        AddOutputLine(" |  __ \\| |                    (_)     / __ \\ / ____|");
        AddOutputLine(" | |__) | |__   ___   ___ _ __  ___  _| |  | | (___  ");
        AddOutputLine(" |  ___/| '_ \\ / _ \\ / _ \\ '_ \\| \\ \\/ / |  | |\\___ \\ ");
        AddOutputLine(" | |    | | | | (_) |  __/ | | | |>  <| |__| |____) |");
        AddOutputLine(" |_|    |_| |_|\\___/ \\___|_| |_|_/_/\\_\\\\____/|_____/ ");
        AddOutputLine("");
        AddOutputLine("        PhoenixOS v2.0 by Kirka");
        AddOutputLine("");
        AddOutputLine("Type 'help' for commands");
        g_welcomeShown = true;
    }
}

LRESULT CALLBACK ShellProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // Показываем приветствие при первом входе
    InitializeWelcome();

    switch(msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            if (g_hFont) {
                SelectObject(hdc, g_hFont);
            }

            if (g_inEditor) {
                DrawEditor(hdc);
            } else {
                // БЕЛЫЙ ТЕКСТ НА ЧЁРНОМ ФОНЕ — как в BIOS/DOS
                SetBkColor(hdc, RGB(0, 0, 0));
                SetTextColor(hdc, RGB(255, 255, 255));

                // Очистка фона
                RECT rc;
                GetClientRect(hwnd, &rc);
                HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
                FillRect(hdc, &rc, hBrush);
                DeleteObject(hBrush);

                // === АВТОМАТИЧЕСКАЯ ПРОКРУТКА КАК В CMD ===
                int lineHeight = 20;
                int maxVisibleLines = rc.bottom / lineHeight - 1; // место для приглашения

                int startLine = 0;
                if (g_outputCount >= maxVisibleLines) {
                    startLine = g_outputCount - maxVisibleLines + 1;
                }

                // Вывод всех видимых строк
                for (int i = startLine; i < g_outputCount; i++) {
                    int y = (i - startLine) * lineHeight;
                    TextOut(hdc, 10, y, g_outputLines[i], (int)strlen(g_outputLines[i]));
                }

                // Приглашение с текущим путём
                char currentDir[MAX_PATH];
                if (_getcwd(currentDir, MAX_PATH)) {
                    char* displayPath = currentDir;
                    if (strlen(currentDir) > 3) {
                        displayPath = currentDir + 3;
                        if (displayPath[0] == '\0') displayPath = "\\";
                    }
                    char prompt[512];
                    sprintf(prompt, "C:%s> %s", displayPath, g_inputBuffer);
                    int inputY = (g_outputCount - startLine) * lineHeight;
                    TextOut(hdc, 10, inputY, prompt, (int)strlen(prompt));
                } else {
                    char prompt[300];
                    sprintf(prompt, "C:\\> %s", g_inputBuffer);
                    int inputY = (g_outputCount - startLine) * lineHeight;
                    TextOut(hdc, 10, inputY, prompt, (int)strlen(prompt));
                }
            }

            EndPaint(hwnd, &ps);
            break;
        }

        case WM_CHAR: {
            if (g_inEditor) {
                EditorHandleChar(wParam);
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }

            if (wParam == '\r' || wParam == '\n') {
                if (g_inputPos > 0) {
                    g_inputBuffer[g_inputPos] = '\0';
                    AddOutputLine(g_inputBuffer);
                    ExecuteCommand(g_inputBuffer);
                    g_inputPos = 0;
                    g_inputBuffer[0] = '\0';
                }
            } else if (wParam == '\b') {
                if (g_inputPos > 0) {
                    g_inputPos--;
                    g_inputBuffer[g_inputPos] = '\0';
                }
            } else if (wParam >= 32 && wParam <= 126) {
                if (g_inputPos < 255) {
                    g_inputBuffer[g_inputPos] = (char)wParam;
                    g_inputPos++;
                    g_inputBuffer[g_inputPos] = '\0';
                }
            }

            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
            break;
        }

        case WM_KEYDOWN: {
            if (g_inEditor) {
                switch(wParam) {
                    case VK_ESCAPE:
                        g_inEditor = false;
                        ClearOutput();
                        // Приветствие не показываем повторно
                        AddOutputLine("PhoenixOS v2.0");
                        AddOutputLine("Copyright (c) 2026 Kirill");
                        AddOutputLine("");
                        break;

                    case VK_UP:
                        if (g_editorCursorY > 0) g_editorCursorY--;
                        if (g_editorCursorY < g_editorScroll) g_editorScroll = g_editorCursorY;
                        break;

                    case VK_DOWN:
                        if (g_editorCursorY < g_editorLineCount) g_editorCursorY++;
                        if (g_editorCursorY >= g_editorScroll + 18) g_editorScroll = g_editorCursorY - 17;
                        break;

                    case VK_F2:
                        SaveFile();
                        break;
                }
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}