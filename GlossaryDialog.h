#include <map>
#include "ui_GlossaryDialog.h"
#include "QDialog"
#include "QString"
#include "QDialog"
#include "QPushButton"
#include "QLineEdit"
#include "QTableView"
#include "GlossaryModel.h"
#include "NewGlossaryDialog.h"
#include "qonlinetranslator.h"
class GlossaryDialog: public QDialog
{
	Q_OBJECT
public:
	GlossaryDialog(std::pair<QOnlineTranslator::Language, QOnlineTranslator::Language> languages);
	~GlossaryDialog();
	void addRow(bool clicked);
	void deleteRow(bool clicked);
	void editTable(const QModelIndex& index);
	void checkEmptyWord(const QModelIndex& index);
	std::vector<std::pair<QString, QString>> getGlossary();
	void setSourceLanguage(QOnlineTranslator::Language lan);
	void setTargetLanguage(QOnlineTranslator::Language lan);

signals:
	void onSaveDictionary();
	void onLoadDictionary();
	void onAddEntry(QString key, QString value);
	void onDeleteEntry(QString key, QString value);

private:
	Ui::GlossaryClass ui;
	QTableView* m_tableView;
	QPushButton* m_newButton;
	QPushButton* m_deleteButton;
	QPushButton* m_saveButton;
	QPushButton* m_loadButton;
	QLineEdit* m_sourceLanLineEdit;
	QLineEdit* m_targetLanLineEdit;
	GlossaryModel* m_model = nullptr;
	NewGlossaryDialog* m_newGlossaryDialog = nullptr;
};
