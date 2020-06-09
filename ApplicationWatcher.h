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
	std::shared_ptr<PIX> capture(RECT roi);
	RECT getWindowSize();
	std::vector<appInfo> getAppInfoList();

private:
	HWND m_appHandle;
};



