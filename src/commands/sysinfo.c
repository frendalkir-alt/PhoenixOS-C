#define _CRT_SECURE_NO_WARNINGS
#include "../../include/phoenixos.h"

// Явно объявляем структуры (на случай, если windows.h их не включил)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601 // Windows 7+
#endif

#include <windows.h> // ← Гарантируем подключение

void cmd_sysinfo() {
    AddOutputLine("System Information");
    AddOutputLine("");

    SYSTEM_INFO si;
    GetNativeSystemInfo(&si); // ← Используем Native!

    char line[300];
    const char* arch = "Unknown";
    switch(si.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_INTEL:
            arch = "x86 (32-bit)";
            break;
        case PROCESSOR_ARCHITECTURE_AMD64:
            arch = "x64 (64-bit)";
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            arch = "ARM";
            break;
        case PROCESSOR_ARCHITECTURE_ARM64:
            arch = "ARM64";
            break;
        default:
            arch = "Other";
    }
    sprintf(line, "Processor Architecture: %s", arch);
    AddOutputLine(line);

    sprintf(line, "Number of Processors: %d", si.dwNumberOfProcessors);
    AddOutputLine(line);

    sprintf(line, "Page Size: %lu bytes", si.dwPageSize);
    AddOutputLine(line);

    MEMORYSTATUS ms;
    GlobalMemoryStatus(&ms);
    sprintf(line, "Total Physical Memory: %lu MB", ms.dwTotalPhys / (1024 * 1024));
    AddOutputLine(line);
    sprintf(line, "Available Memory: %lu MB", ms.dwAvailPhys / (1024 * 1024));
    AddOutputLine(line);
}