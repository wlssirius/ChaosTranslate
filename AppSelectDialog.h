#include "QDialog"
#include "ui_SelectApp.h"
#include "QListWidget"
#include "QPushButton"
#include <string>
#include <vector>

class AppSelectDialog : public QDialog
{
	Q_OBJECT

public:
	AppSelectDialog(std::vector<std::pair<std::string, HICON>> appList);

signals:
	void selectApp(QString title);

public slots:
	void onPressOK(bool clicked);

private:
	Ui::SelectApp ui;
	QListWidget* m_listWidget;
	QPushButton* m_ok;
	QPushButton* m_cancel;
};