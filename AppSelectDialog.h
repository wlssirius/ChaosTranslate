#include "QDialog"
#include "ui_SelectApp.h"
#include "QListWidget"
#include "QPushButton"
#include <string>
#include <vector>

class AppSelectDialog : public QDialog
{
public:
	AppSelectDialog(std::vector<std::pair<std::string, HICON>> appList);

private:
	Ui::SelectApp ui;
	QListWidget* m_listWidget;
	QPushButton* m_ok;
	QPushButton* m_cancel;
};