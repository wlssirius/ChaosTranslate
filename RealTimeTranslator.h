#include <QtWidgets/QMainWindow>
#include "ui_RealTimeTranslator.h"
#include "qpushbutton.h"
#include "qtextedit.h"
#include "qonlinetranslator.h"
#include "qrubberband.h"
#include "ApplicationWatcher.h"
#include "InvisibleCanvas.h"

class RealTimeTranslator : public QMainWindow
{
	Q_OBJECT

public:
	RealTimeTranslator(QWidget *parent = Q_NULLPTR);

	void translate(bool clicked);
	void selectRoi(bool clicked);
	void setRoi(RECT roi) { m_roi = roi; }

private:
	Ui::RealTimeTranslatorClass ui;
	QPushButton* m_captureButton;
	QPushButton* m_roiButton;
	QTextEdit* m_originalTextEdit;
	QTextEdit* m_translateTextEdit;
	QOnlineTranslator m_translator;

	ApplicationWatcher m_watcher;
	RECT m_roi;

};
