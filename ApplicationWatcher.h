
#ifndef APPLICATION_WATCHER_HEADER
#define APPLICATION_WATCHER_HEADER

#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#include <WinUser.h>
#include <iostream>
#include <stdio.h>
#include <wchar.h>
#include <vector>
#include <leptonica/allheaders.h>
#include "QString"
#include "memory"

class ApplicationWatcher
{
public:
	using appInfo = std::pair<std::string, HICON>;
	ApplicationWatcher(HWND handle = nullptr) { m_appHandle = handle; }

	void setApplication(QString windowTitle);
	PIX* capture(RECT roi);
	RECT getWindowSize();
	std::vector<appInfo> getAppInfoList();
	bool appSelected() { return m_appHandle != nullptr; }

private:
	HWND m_appHandle;
};

#endif