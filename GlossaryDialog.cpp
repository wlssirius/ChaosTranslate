#include "GlossaryDialog.h"

GlossaryDialog::GlossaryDialog(std::pair<QOnlineTranslator::Language, QOnlineTranslator::Language> languages):
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
	m_sourceLanLineEdit = findChild<QLineEdit*>("sourceLanguageLineEdit");
	m_targetLanLineEdit = findChild<QLineEdit*>("targetLanguageLineEdit");

	QString sourceLanguage = QVariant::fromValue(languages.first).toString();
	QString targetLanguage = QVariant::fromValue(languages.second).toString();
	m_sourceLanLineEdit->setText(sourceLanguage);
	m_targetLanLineEdit->setText(targetLanguage);

	connect(m_newButton, &QPushButton::clicked, this, &GlossaryDialog::addRow);
	connect(m_deleteButton, &QPushButton::clicked, this, &GlossaryDialog::deleteRow);
	m_tableView->setModel(m_model); 
	m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

GlossaryDialog::~GlossaryDialog()
{
	if (m_newGlossaryDialog != nullptr)
	{
		delete m_newGlossaryDialog;
	}
	if (m_model != nullptr)
	{
		delete m_model;
	}
}

void GlossaryDialog::addRow(bool clicked)
{
	//int rowCount = m_model->getGlossaryCount();
	//m_model->insertRow(rowCount);
	//QModelIndex index = m_model->index(rowCount, 0);
	//editTable(index);
	if (m_newGlossaryDialog != nullptr)
	{
		delete m_newGlossaryDialog;
	}
	m_newGlossaryDialog = new NewGlossaryDialog();
	m_newGlossaryDialog->show();
	connect(m_newGlossaryDialog, &NewGlossaryDialog::addGlossary, m_model, &GlossaryModel::addEntry);
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

std::vector<std::pair<QString, QString>> GlossaryDialog::getGlossary()
{
	if (m_model != nullptr)
	{
		return m_model->getGlossary();
	}
	return std::vector<std::pair<QString, QString>>();
}
