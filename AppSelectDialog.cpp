#include "AppSelectDialog.h"
#include "QPixmap"
#include <QtWinExtras\QtWin>

using namespace std;

AppSelectDialog::AppSelectDialog(std::vector<ApplicationWatcher::appInfo> appList)
{
	ui.setupUi(this);
	m_listWidget = findChild<QListWidget*>("appList");
	m_ok = findChild<QPushButton*>("okButton");
	m_cancel = findChild<QPushButton*>("cancelButton");
	setModal(true);

	connect(m_ok, &QPushButton::clicked, this, &AppSelectDialog::onPressOK);

	for (auto appInfo : appList)
	{
		AppItem* item = new AppItem(appInfo.ptr);
		auto pixmap = QtWin::fromHICON(appInfo.icon);
		item->setIcon(pixmap);
		QString title = QString::fromLocal8Bit(appInfo.name.c_str());
		item->setText(title);
		m_listWidget->addItem(item);
	}
}

void AppSelectDialog::onPressOK(bool clicked)
{
	auto items = m_listWidget->selectedItems();
	if (items.size() > 0)
	{
		AppItem* item = dynamic_cast<AppItem*>(items[0]);
		emit selectApp(item->getHWnd());
	}
	destroy(true);
}