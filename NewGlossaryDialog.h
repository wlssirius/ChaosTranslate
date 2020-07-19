
#ifndef NEW_GLOSSARY_DIALOG_HEADER
#define NEW_GLOSSARY_DIALOG_HEADER

#include "QDialog"
#include "QPushbutton"
#include "QLineEdit"
#include "ui_NewGlossary.h"

class NewGlossaryDialog : public QDialog
{
	Q_OBJECT
public:
	NewGlossaryDialog();
signals:
	void addGlossary(QString original, QString translate);
private:
	void createEntry(bool clicked);
	Ui::NewGlossaryClass ui;
	QPushButton* m_okButton;
	QPushButton* m_cancelButton;
	QLineEdit* m_original;
	QLineEdit* m_translate;
};

#endif