#define _CRT_SECURE_NO_WARNINGS
#include "../include/phoenixos.h"

HWND g_hwnd = NULL;
HFONT g_hFont = NULL;
char g_inputBuffer[256] = {0};
int g_inputPos = 0;
char g_outputLines[500][256];
int g_outputCount = 0;

// Режимы BIOS
enum {
    BIOS_MENU,
    BIOS_LOADING_PHOENIX,
    BIOS_LOADING_WINDOWS
};
static int g_biosMode = BIOS_MENU;
static int g_biosSelection = 0; // 0 = PhoenixOS, 1 = Windows
static UINT_PTR g_biosTimer = 0;
static int g_countdown = 5;

// Центрированный вывод
void DrawCenteredText(HDC hdc, const char* text, int y) {
    SIZE size;
    GetTextExtentPoint32(hdc, text, (int)strlen(text), &size);
    int x = (GetSystemMetrics(SM_CXSCREEN) - size.cx) / 2;
    TextOut(hdc, x, y, text, (int)strlen(text));
}

LRESULT CALLBACK BiosProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
            g_biosTimer = SetTimer(hwnd, 1, 1000, NULL);
            break;

        case WM_TIMER: {
            if (g_biosMode == BIOS_MENU) {
                if (g_countdown > 0) {
                    g_countdown--;
                    InvalidateRect(hwnd, NULL, TRUE);
                } else {
                    g_biosMode = BIOS_LOADING_PHOENIX;
                    KillTimer(hwnd, g_biosTimer);
                    g_biosTimer = SetTimer(hwnd, 2, 2000, NULL);
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            } else if (g_biosMode == BIOS_LOADING_PHOENIX || g_biosMode == BIOS_LOADING_WINDOWS) {
                KillTimer(hwnd, g_biosTimer);
                if (g_biosMode == BIOS_LOADING_PHOENIX) {
                    SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)ShellProc);
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                } else {
                    PostQuitMessage(0);
                }
            }
            break;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            if (g_hFont) SelectObject(hdc, g_hFont);

            SetBkColor(hdc, RGB(0, 0, 0));
            SetTextColor(hdc, RGB(255, 255, 255));

            RECT rc = {0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
            FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

            if (g_biosMode == BIOS_MENU) {
                // === ТОЛЬКО ЦЕНТРИРОВАНИЕ — ВСЁ ОСТАЛЬНОЕ КАК БЫЛО ===
                int boxW = 600;
                int boxH = 180;
                int boxX = (GetSystemMetrics(SM_CXSCREEN) - boxW) / 2;
                int boxY = (GetSystemMetrics(SM_CYSCREEN) - boxH) / 2 - 50;

                // Рамка
                HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
                HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
                HGDIOBJ oldPen = SelectObject(hdc, hPen);
                HGDIOBJ oldBrush = SelectObject(hdc, hBrush);

                Rectangle(hdc, boxX, boxY, boxX + boxW, boxY + boxH);

                DeleteObject(hPen);
                DeleteObject(hBrush);

                // Заголовок внутри рамки
                SetTextColor(hdc, RGB(255, 255, 255));
                TextOut(hdc, boxX + 10, boxY + 10, "Dual boot manager", 16);

                // Список ОС
                char osList[2][100];
                sprintf(osList[0], "PhoenixOS v2.0");
                sprintf(osList[1], "Windows");

                for (int i = 0; i < 2; i++) {
                    int y = boxY + 40 + i * 25;
                    if (i == g_biosSelection) {
                        // Выделенная строка — серый фон
                        HBRUSH hGray = CreateSolidBrush(RGB(128, 128, 128));
                        RECT rect = {boxX + 10, y - 2, boxX + boxW - 10, y + 20};
                        FillRect(hdc, &rect, hGray);
                        DeleteObject(hGray);
                        SetTextColor(hdc, RGB(255, 255, 255)); // Чёрный текст на сером
                    } else {
                        SetTextColor(hdc, RGB(255, 255, 255)); // Белый текст
                    }
                    TextOut(hdc, boxX + 20, y, osList[i], (int)strlen(osList[i]));
                }

                // Подсказки под рамкой — ТОЧНО КАК У ТЕБЯ
                SetTextColor(hdc, RGB(255, 255, 255));
                TextOut(hdc, boxX, boxY + boxH + 20,
                    "Use the up and down keys to select which entry is highlighted.", 62);
                TextOut(hdc, boxX, boxY + boxH + 40,
                    "Press Enter to boot the selected OS, 'e' to edit the commands before booting, or 'c' for a command-line.", 95);
                TextOut(hdc, boxX, boxY + boxH + 80,
                    "The highlighted entry will be booted automatically in 4 seconds.", 64);
            } else if (g_biosMode == BIOS_LOADING_PHOENIX) {
                DrawCenteredText(hdc, "Starting PhoenixOS...", 200);
            } else if (g_biosMode == BIOS_LOADING_WINDOWS) {
                DrawCenteredText(hdc, "Loading Windows...", 200);
            }

            EndPaint(hwnd, &ps);
            break;
        }

        case WM_KEYDOWN: {
            if (g_biosMode != BIOS_MENU) return 0;

            if (g_biosTimer) {
                KillTimer(hwnd, g_biosTimer);
                g_biosTimer = 0;
            }

            switch(wParam) {
                case VK_UP:
                    g_biosSelection = 0;
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case VK_DOWN:
                    g_biosSelection = 1;
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case VK_RETURN:
                    if (g_biosSelection == 0) {
                        g_biosMode = BIOS_LOADING_PHOENIX;
                        g_biosTimer = SetTimer(hwnd, 2, 2000, NULL);
                    } else {
                        g_biosMode = BIOS_LOADING_WINDOWS;
                        g_biosTimer = SetTimer(hwnd, 2, 2000, NULL);
                    }
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
            }
            break;
        }

        case WM_DESTROY:
            if (g_biosTimer) KillTimer(hwnd, g_biosTimer);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nCmdShow) {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = BiosProc;
    wc.hInstance = hInst;
    wc.lpszClassName = "PhoenixOS";
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hCursor = NULL;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wc);

    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    g_hwnd = CreateWindowEx(
        WS_EX_TOPMOST,
        "PhoenixOS",
        "PhoenixOS Boot",
        WS_POPUP | WS_VISIBLE,
        0, 0, w, h,
        NULL, NULL, hInst, NULL
    );

    g_hFont = CreateFont(
        16, 0, 0, 0,
        FW_NORMAL,
        FALSE, FALSE, FALSE,
        OEM_CHARSET,
        OUT_RASTER_PRECIS,
        CLIP_DEFAULT_PRECIS,
        NONANTIALIASED_QUALITY,
        FIXED_PITCH | FF_DONTCARE,
        "Courier New"
    );

    ShowWindow(g_hwnd, nCmdShow);
    UpdateWindow(g_hwnd);
    ShowCursor(FALSE);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (g_hFont) DeleteObject(g_hFont);
    ShowCursor(TRUE);
    return (int)msg.wParam;
}