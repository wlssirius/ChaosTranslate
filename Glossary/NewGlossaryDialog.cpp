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
