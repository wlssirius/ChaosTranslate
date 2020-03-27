#include "RealTimeTranslator.h"
#include <qpushbutton.h>
#include "characterrecognize.h"
#include "qdialog.h"

RealTimeTranslator::RealTimeTranslator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

    m_captureButton = findChild<QPushButton*>("pushButton");
	connect(m_captureButton, &QPushButton::clicked, this, &RealTimeTranslator::captureAndTranslate);
	m_roiButton = findChild<QPushButton*>("pushButton_2");
	connect(m_roiButton, &QPushButton::clicked, this, &RealTimeTranslator::selectRoi);
	m_translateButton = findChild<QPushButton*>("pushButton_3");
	connect(m_translateButton, &QPushButton::clicked, this, &RealTimeTranslator::translate);
	m_glossaryButton = findChild<QPushButton*>("pushButton_4");
	connect(m_glossaryButton, &QPushButton::clicked, &m_glossary, &Glossary::showGlossaryDialog);
	m_originalTextEdit = findChild<QTextEdit*>("textEdit");
	m_translateTextEdit = findChild<QTextEdit*>("textEdit_2");

	QObject::connect(&m_translator, &QOnlineTranslator::finished, [&] {
		if (this->m_translator.error() == QOnlineTranslator::NoError)
			this->m_translateTextEdit->setText(this->m_translator.translation());
		else
			this->m_translateTextEdit->setText(this->m_translator.errorString());
		});

	m_roi.left = 0;
	m_roi.right = 0;
	m_roi.top = 0;
	m_roi.bottom = 0;

	m_watcher.setApplication(FindWindow(0, L"ƒ‰ƒ“ƒX‚P‚O"));
}

void RealTimeTranslator::captureAndTranslate(bool clicked)
{	
	m_watcher.capture(m_roi);
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

void RealTimeTranslator::translate(bool clicked)
{
	QString original = m_originalTextEdit->toPlainText();
	m_translator.translate(original, QOnlineTranslator::Google, QOnlineTranslator::SimplifiedChinese);
}

void RealTimeTranslator::selectRoi(bool clicked)
{
	auto windowRect = m_watcher.getWindowSize();
	m_watcher.capture(windowRect);
	//m_canvas.showCanvas(windowRect);
	auto canvas = new InvisibleCanvas(this->m_roi);
	canvas->showCanvas(windowRect);
	canvas->show();
}
