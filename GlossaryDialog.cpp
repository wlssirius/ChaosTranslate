#include "GlossaryDialog.h"

GlossaryDialog::GlossaryDialog():
	QDialog()
{
	ui.setupUi(this);
	m_tableView = findChild<QTableView*>("tableView");
	if (m_model == nullptr)
	{
		m_model = new GlossaryModel();
	}
	
	m_tableView->setModel(m_model);
	//m_tableView->show();
}

