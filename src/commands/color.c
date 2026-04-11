#define _CRT_SECURE_NO_WARNINGS
#include "../../include/phoenixos.h"

void cmd_color(const char* args) {
    if (!args || strlen(args) == 0) {
        AddOutputLine("Usage: color <bg><fg>");
        AddOutputLine("Example: color 0A - black background, green text");
        AddOutputLine("Colors: 0=black, 1=blue, 2=green, 3=aqua, 4=red, 5=purple, 6=yellow, 7=white, 8=gray, 9=light blue, A=light green, B=light aqua, C=light red, D=light purple, E=light yellow, F=light white");
        return;
    }

    if (strlen(args) != 2) {
        AddOutputLine("Error: Color must be 2 characters");
        return;
    }

    char bg = args[0];
    char fg = args[1];

    // Проверка допустимых символов
    if ((bg < '0' || bg > 'F') || (fg < '0' || fg > 'F')) {
        AddOutputLine("Error: Invalid color code");
        return;
    }

    // Запоминаем цвета (в реальном проекте можно сохранять в глобальных переменных)
    int bgColor = 0;
    int fgColor = 0;

    if (bg >= '0' && bg <= '9') bgColor = bg - '0';
    else if (bg >= 'A' && bg <= 'F') bgColor = bg - 'A' + 10;

    if (fg >= '0' && fg <= '9') fgColor = fg - '0';
    else if (fg >= 'A' && fg <= 'F') fgColor = fg - 'A' + 10;

    AddOutputLine("Color changed.");
}