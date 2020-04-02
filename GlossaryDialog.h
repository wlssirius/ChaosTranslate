#include <map>
#include "ui_GlossaryDialog.h"
#include "QDialog"
#include "QString"
#include "QDialog"
#include "QPushButton"
#include "QTableView"
#include "GlossaryModel.h"
class GlossaryDialog: public QDialog
{
	Q_OBJECT
public:
	GlossaryDialog();
	void deleteRow();
private:
	Ui::GlossaryClass ui;
	QTableView* m_tableView;
	QPushButton* m_newButton;
	QPushButton* m_deleteButton;
	GlossaryModel* m_model = nullptr;
};
