#include "GlossaryDialog.h"

GlossaryDialog::GlossaryDialog():
	QDialog()
{
	ui.setupUi(this);
	m_tableView = findChild<QTableView*>("tableView");
	m_table
}

