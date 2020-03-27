#include <map>
#include "ui_Glossary.h"
#include "QDialog"
#include "QString"
#include "QDialog"
class Glossary: public QDialog
{
	Q_OBJECT
public:
	Glossary();
	void showGlossaryDialog();

private:
	Ui::GlossaryClass ui;
	std::map<QString, QString> m_dictionary;

};
