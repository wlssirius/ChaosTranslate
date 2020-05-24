#include "AppSelectDialog.h"
#include "QPixmap"
#include <QtWinExtras\QtWin>

using namespace std;

AppSelectDialog::AppSelectDialog(std::vector<std::pair<std::string, HICON>> appList)
{
	ui.setupUi(this);
	m_listWidget = findChild<QListWidget*>("appList");
	m_ok = findChild<QPushButton*>("okButton");
	m_cancel = findChild<QPushButton*>("cancelButton");

	connect(m_ok, &QPushButton::clicked, this, &AppSelectDialog::onPressOK);

	for (auto appInfo : appList)
	{
		QListWidgetItem* item = new QListWidgetItem;
		auto pixmap = QtWin::fromHICON(appInfo.second);
		item->setIcon(pixmap);
		QString title = QString::fromLocal8Bit(appInfo.first.c_str());
		item->setText(title);
		m_listWidget->addItem(item);
	}
}

void AppSelectDialog::onPressOK(bool clicked)
{
	auto item = m_listWidget->selectedItems();
	emit selectApp(item[0]->text());
}