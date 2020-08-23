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

#include "NewGlossaryDialog.h"
#include "QMessageBox"

NewGlossaryDialog::NewGlossaryDialog()
{
	ui.setupUi(this);

	m_okButton = findChild<QPushButton*>("okButton");
	m_cancelButton = findChild<QPushButton*>("cancelButton");
	m_original = findChild<QLineEdit*>("lineEdit");
	m_translate = findChild<QLineEdit*>("lineEdit_2");

	connect(m_okButton, &QPushButton::clicked, this, &NewGlossaryDialog::createEntry);
	
}

void NewGlossaryDialog::createEntry(bool clicked)
{
	if (m_original->text().size() == 0)
	{
		QMessageBox msg;
		msg.setText("Original Text shouldn't be empty");
		msg.exec(); 
	}
	else
	{
		emit addGlossary(m_original->text(), m_translate->text()); 
	}
}
