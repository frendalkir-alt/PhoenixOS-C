#define _CRT_SECURE_NO_WARNINGS
#include "../../include/phoenixos.h"

void cmd_reboot() {
    AddOutputLine("Rebooting PhoenixOS...");
    Sleep(1500);

    // Очищаем историю и буфер
    ClearOutput();

    // Перерисовываем экран
    InvalidateRect(g_hwnd, NULL, TRUE);
    UpdateWindow(g_hwnd);
}