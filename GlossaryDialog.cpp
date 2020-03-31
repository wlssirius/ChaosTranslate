#include "GlossaryDialog.h"

GlossaryDialog::GlossaryDialog():
	QDialog()
{
	ui.setupUi(this);

	if (m_model == nullptr)
	{
		m_model = new GlossaryModel();
	}

	m_tableView = findChild<QTableView*>("tableView");
	m_newButton = findChild<QPushButton*>("pushButton");
	m_deleteButton = findChild<QPushButton*>("pushButton_2");

	connect(m_newButton, &QPushButton::clicked, m_model, &GlossaryModel::addNewRow);
	m_tableView->setModel(m_model);
}

