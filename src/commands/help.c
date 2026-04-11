#include "../../include/phoenixos.h"
#define _CRT_SECURE_NO_WARNINGS

void cmd_help() {
        AddOutputLine("Available commands:");
        AddOutputLine("  cls       - Clear screen");
        AddOutputLine("  dir       - List files");
        AddOutputLine("  cd        - Change directory");
        AddOutputLine("  help      - Show this help");
        AddOutputLine("  exit      - Shutdown system");
        AddOutputLine("  brows     - Text browser (brows http://example.com)");
        AddOutputLine("  playmp3   - Play mp3 file");
        AddOutputLine("  stop      - Stop MP3 playback");
        AddOutputLine("  echo      - Print text to screen");
        AddOutputLine("  ver       - Show version info");
        AddOutputLine("  sysinfo   - Show system information");
        AddOutputLine("  tree      - Display directory structure");
        AddOutputLine("  copy      - Copy file");
        AddOutputLine("  delete    - Delete file");
        AddOutputLine("  edit      - Edit file with Notepad");
        AddOutputLine("  mkdir     - Create directory");
        AddOutputLine("  mklink    - Create symbolic link (not supported)");
        AddOutputLine("  rmdir     - Remove directory");
        AddOutputLine("  color     - Change text/background color");
        AddOutputLine("  time      - Show current time");
        AddOutputLine("  date      - Show current date");
        AddOutputLine("  reboot    - Simulate system reboot");
        AddOutputLine("  calc      - Simple calculator");
        }