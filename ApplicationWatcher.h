
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
	struct appInfo {
		std::string name;
		HICON icon;
		HWND ptr;
	};

	enum class ErrorCode
	{
		APP_CLOSED,
		APP_MINIMIZED
	};
	ApplicationWatcher(HWND handle = nullptr) { m_appHandle = handle; }

	void setApplication(HWND windowPtr);
	PIX* capture();
	RECT getWindowSize();
	std::vector<appInfo> getAppInfoList();
	bool appSelected();
	ErrorCode getErrorCode() { return m_errorCode; }

private:
	HWND m_appHandle;
	ErrorCode m_errorCode;
};

#endif