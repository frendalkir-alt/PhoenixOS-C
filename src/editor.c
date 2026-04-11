#define _CRT_SECURE_NO_WARNINGS
#include "../include/phoenixos.h"
#include <stdio.h>

// === Глобальные переменные редактора (ОПРЕДЕЛЕНИЯ) ===
bool g_inEditor = false;
int g_editorScroll = 0;
int g_editorLineCount = 0;
int g_editorCursorY = 0; // Текущая строка для редактирования

static char g_editFilename[256] = {0};
static char g_editorLines[200][256];
static char g_editorInput[256] = {0}; // Буфер ввода текущей строки
static int g_editorInputPos = 0;

// Загрузка файла
void LoadFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        // Создаём новый файл
        strncpy(g_editFilename, filename, 255);
        g_editFilename[255] = '\0';
        g_editorLineCount = 0;
        g_inEditor = true;
        return;
    }

    strncpy(g_editFilename, filename, 255);
    g_editFilename[255] = '\0';
    g_editorLineCount = 0;

    char line[256];
    while (fgets(line, sizeof(line), file) && g_editorLineCount < 200) {
        int len = (int)strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        strncpy(g_editorLines[g_editorLineCount], line, 255);
        g_editorLines[g_editorLineCount][255] = '\0';
        g_editorLineCount++;
    }
    fclose(file);
    g_inEditor = true;
}

// Сохранение файла
void SaveFile() {
    FILE* file = fopen(g_editFilename, "w");
    if (!file) return;

    for (int i = 0; i < g_editorLineCount; i++) {
        fprintf(file, "%s\n", g_editorLines[i]);
    }
    fclose(file);
}

// Обработка ввода в редакторе
void EditorHandleChar(WPARAM wParam) {
    // Ctrl+O = ASCII 0x0F → сохранение
    if (wParam == 0x0F) {
        SaveFile();
        return;
    }

    if (wParam == '\r' || wParam == '\n') {
        // Enter: сохранить текущую строку
        if (g_editorCursorY < g_editorLineCount) {
            strcpy(g_editorLines[g_editorCursorY], g_editorInput);
        } else {
            if (g_editorLineCount < 200) {
                strcpy(g_editorLines[g_editorLineCount], g_editorInput);
                g_editorLineCount++;
            }
        }
        g_editorInput[0] = '\0';
        g_editorInputPos = 0;
        g_editorCursorY++;
        // Автопрокрутка вниз
        if (g_editorCursorY > g_editorScroll + 17) {
            g_editorScroll = g_editorCursorY - 17;
        }
    } else if (wParam == '\b') {
        // Backspace
        if (g_editorInputPos > 0) {
            g_editorInputPos--;
            g_editorInput[g_editorInputPos] = '\0';
        }
    } else if (wParam >= 32 && wParam <= 126) {
        // Печатаемые символы
        if (g_editorInputPos < 255) {
            g_editorInput[g_editorInputPos] = (char)wParam;
            g_editorInputPos++;
            g_editorInput[g_editorInputPos] = '\0';
        }
    }
}

// Отображение редактора
void DrawEditor(HDC hdc) {
    RECT rc;
    GetClientRect(g_hwnd, &rc);
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(hdc, &rc, hBrush);
    DeleteObject(hBrush);

    SetTextColor(hdc, RGB(0, 255, 0));
    SetBkColor(hdc, RGB(0, 0, 0));

    // Заголовок
    char title[300];
    snprintf(title, sizeof(title), "Phoenix Editor - %s", g_editFilename);
    TextOut(hdc, 10, 0, title, (int)strlen(title));

    // Отображение существующих строк
    int maxLines = 18;
    for (int i = 0; i < maxLines && (g_editorScroll + i) < g_editorLineCount; i++) {
        int y = (i + 1) * 20;
        COLORREF color = (g_editorScroll + i == g_editorCursorY) ? RGB(255, 255, 0) : RGB(0, 255, 0);
        SetTextColor(hdc, color);
        TextOut(hdc, 10, y, g_editorLines[g_editorScroll + i],
                (int)strlen(g_editorLines[g_editorScroll + i]));
    }

    // Строка ввода (если курсор в новой строке)
    if (g_editorCursorY >= g_editorLineCount) {
        int y = (g_editorLineCount - g_editorScroll + 1) * 20;
        SetTextColor(hdc, RGB(0, 255, 255));
        TextOut(hdc, 10, y, g_editorInput, (int)strlen(g_editorInput));
    }

    // Подсказка
    SetTextColor(hdc, RGB(0, 255, 255));
    TextOut(hdc, 10, 400, "Esc: Exit | Ctrl+O: Save | Arrow Keys: Navigate", 48);
}