#include <map>
#include "ui_GlossaryDialog.h"
#include "QDialog"
#include "QString"
#include "QDialog"
#include "qtableview.h"
#include "GlossaryModel.h"
class GlossaryDialog: public QDialog
{
	Q_OBJECT
public:
	GlossaryDialog();

private:
	Ui::GlossaryClass ui;
	QTableView* m_tableView;
	GlossaryModel* m_model = nullptr;
};
