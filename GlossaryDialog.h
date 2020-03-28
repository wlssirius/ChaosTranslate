#include <map>
#include "ui_GlossaryDialog.h"
#include "QDialog"
#include "QString"
#include "QDialog"
class GlossaryDialog: public QDialog
{
	Q_OBJECT
public:
	GlossaryDialog();

private:
	Ui::GlossaryClass ui;
	std::map<QString, QString> m_dictionary;

};
