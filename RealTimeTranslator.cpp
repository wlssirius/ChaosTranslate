#include "RealTimeTranslator.h"
#include <qpushbutton.h>
#include "characterrecognize.h"
#include "qdialog.h"

RealTimeTranslator::RealTimeTranslator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

    m_captureButton = findChild<QPushButton*>("pushButton");
	connect(m_captureButton, &QPushButton::clicked, this, &RealTimeTranslator::translate);
	m_roiButton = findChild<QPushButton*>("pushButton_2");
	connect(m_roiButton, &QPushButton::clicked, this, &RealTimeTranslator::setRoi);
	m_originalTextEdit = findChild<QTextEdit*>("textEdit");
	m_translateTextEdit = findChild<QTextEdit*>("textEdit_2");

	QObject::connect(&m_translator, &QOnlineTranslator::finished, [&] {
		if (this->m_translator.error() == QOnlineTranslator::NoError)
			this->m_translateTextEdit->setText(this->m_translator.translation());
		else
			this->m_translateTextEdit->setText(this->m_translator.errorString());
		});

	m_watcher.setApplication(FindWindow(0, L"ƒ‰ƒ“ƒX‚P‚O"));
}

void RealTimeTranslator::translate(bool clicked)
{
	m_watcher.screenshot_window();
	//QString test("hello world");
	QString capture = ocr();
	QStringList list1 = capture.split('\n');
	QString simplified;
	for (auto str : list1)
	{
		if (str.size() == 0)
		{
			continue;
		}
		if (str == 32)
		{
			continue;
		}
		simplified.append(str);
		simplified.append('\n');
	}
	m_originalTextEdit->setText(simplified);
	m_translator.translate(simplified, QOnlineTranslator::Google, QOnlineTranslator::SimplifiedChinese); 
}

void RealTimeTranslator::setRoi(bool clicked)
{
	QDialog* dialog = new QDialog(this, Qt::WindowTransparentForInput|Qt::WindowStaysOnTopHint);
	dialog->show();
}
