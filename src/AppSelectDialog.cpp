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
		QString title = QString::fromUtf16((const ushort*)(appInfo.name.c_str()));

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