#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#include <WinUser.h>
#include <iostream>
#include <stdio.h>
#include <wchar.h>
#include <vector>

class ApplicationWatcher
{
public:
	ApplicationWatcher(HWND handle = nullptr) { m_appHandle = handle; }

	void setApplication(HWND handle) { m_appHandle = handle; }
	void capture(RECT roi);
	RECT getWindowSize();

private:
	HWND m_appHandle;
};



