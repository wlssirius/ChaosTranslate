#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#include <WinUser.h>
#include <iostream>
#include <stdio.h>
#include <wchar.h>
#include <vector>

void screenshot_window(HWND handle);

BOOL CALLBACK speichereFenster(HWND hwnd, LPARAM lParam);

void capture();
