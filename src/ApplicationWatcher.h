/*
*   Copyright (C) 2019-2020  Wei Lisi (Willis) <weilisi16@gmail.com>
*	This file is part of ChaosTranslate
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
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