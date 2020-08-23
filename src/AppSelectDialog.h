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
#ifndef APP_SELECT_DIALOG_HEADER
#define APP_SELECT_DIALOG_HEADER

#include "ApplicationWatcher.h"
#include "QDialog"
#include "ui_SelectApp.h"
#include "QListWidget"
#include "QPushButton"
#include <string>
#include <vector>

class AppItem : public QListWidgetItem
{
public:
	AppItem(HWND hWnd) :
		m_hWnd(hWnd), QListWidgetItem() {}
	HWND getHWnd() { return m_hWnd; }
private:
	HWND m_hWnd;
};

class AppSelectDialog : public QDialog
{
	Q_OBJECT

public:
	AppSelectDialog(std::vector<ApplicationWatcher::appInfo> appList);

signals:
	void selectApp(HWND hWnd);

public slots:
	void onPressOK(bool clicked);

private:
	Ui::SelectApp ui;
	QListWidget* m_listWidget;
	QPushButton* m_ok;
	QPushButton* m_cancel;
};

#endif