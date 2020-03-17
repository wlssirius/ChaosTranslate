#include "RealTimeTranslator.h"
#include "qonlinetranslator.h"
#include "screencapture.h"
#include <qpushbutton.h>
#include "characterrecognize.h"

RealTimeTranslator::RealTimeTranslator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QOnlineTranslator translator;

    auto button = findChild<QPushButton*>("pushButton");
	connect(button, &QPushButton::clicked, this, &RealTimeTranslator::translate);
	m_originalTextEdit = findChild<QTextEdit*>("textEdit");
}

void RealTimeTranslator::translate(bool clicked)
{
	capture();
	std::string string = ocr(m_originalTextEdit);
}
