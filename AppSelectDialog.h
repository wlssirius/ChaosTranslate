
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