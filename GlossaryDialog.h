#include <map>
#include "ui_GlossaryDialog.h"
#include "QDialog"
#include "QString"
#include "QDialog"
#include "qtableview.h"
class GlossaryDialog: public QDialog
{
	Q_OBJECT
public:
	GlossaryDialog();

private:
	Ui::GlossaryClass ui;
	QTableView* m_tableView;
};
