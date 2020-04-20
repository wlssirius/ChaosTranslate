#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#include <WinUser.h>
#include <iostream>
#include <stdio.h>
#include <wchar.h>
#include <vector>
#include <leptonica/allheaders.h>

class ApplicationWatcher
{
public:
	ApplicationWatcher(HWND handle = nullptr) { m_appHandle = handle; }

	void setApplication(HWND handle) { m_appHandle = handle; }
	PIX* capture(RECT roi);
	RECT getWindowSize();

private:
	HWND m_appHandle;
};



