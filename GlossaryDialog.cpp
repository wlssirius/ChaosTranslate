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

	connect(m_newButton, &QPushButton::clicked, this, &GlossaryDialog::addRow);
	connect(m_deleteButton, &QPushButton::clicked, this, &GlossaryDialog::deleteRow);
	connect(m_model, &GlossaryModel::editCell, this, &GlossaryDialog::editTable);
	m_tableView->setModel(m_model); 
	m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void GlossaryDialog::addRow(bool clicked)
{
	int rowCount = m_model->getGlossaryCount();
	m_model->insertRow(rowCount);
	QModelIndex index = m_model->index(rowCount, 0);
	editTable(index);
}

void GlossaryDialog::deleteRow(bool clicked)
{
	QItemSelectionModel* select = m_tableView->selectionModel();
	auto selectedRows = select->selectedRows();
	for (auto row : selectedRows)
	{
		m_model->removeRow(row.row());
	}
}

void GlossaryDialog::editTable(const QModelIndex& index)
{
	m_tableView->edit(index);
}

void GlossaryDialog::checkEmptyWord(const QModelIndex& index)
{
}
