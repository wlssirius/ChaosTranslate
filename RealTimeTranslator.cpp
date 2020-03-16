#include "RealTimeTranslator.h"
#include "qonlinetranslator.h"

RealTimeTranslator::RealTimeTranslator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QOnlineTranslator translator;
}
