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
	connect(m_glossaryButton, &QPushButton::clicked, &m_glossary, &GlossaryManager::showDialog);
	m_originalTextEdit = findChild<QTextEdit*>("textEdit");
	m_translateTextEdit = findChild<QTextEdit*>("textEdit_2");


	m_roi.left = 0;
	m_roi.right = 0;
	m_roi.top = 0;
	m_roi.bottom = 0;

	m_watcher.setApplication(FindWindow(0, L"ƒ‰ƒ“ƒX‚P‚O"));
}

void RealTimeTranslator::captureAndTranslate(bool clicked)
{
	PIX* pix = m_watcher.capture(m_roi);
	BOX* roi = new Box();
	roi->x = m_roi.left;
	roi->y = m_roi.top;
	roi->w = m_roi.right-m_roi.left;
	roi->h = m_roi.bottom - m_roi.top;
	PIX* th;
	if (roi->w == 0 || roi->h == 0)
	{
		th = pix;
	}
	else
	{
		th = threshold(pix, roi);
	}
	QString capture = ocr(th);
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
	translate(true);
}

void RealTimeTranslator::translate(bool clicked)
{
	QString original = m_originalTextEdit->toPlainText();
	std::map<QString, QString> dict;
	auto encoded = m_glossary.encode(original, dict);
	m_translator.translate(encoded, QOnlineTranslator::Google, QOnlineTranslator::SimplifiedChinese);

	QObject::connect(&m_translator, &QOnlineTranslator::finished, [=] {
		if (this->m_translator.error() == QOnlineTranslator::NoError)
		{
			auto translation = this->m_translator.translation();
			auto decoded = m_glossary.decode(translation, dict);
			this->m_translateTextEdit->setText(decoded);
		}
		else
		{
			this->m_translateTextEdit->setText(this->m_translator.errorString());
		}
		});
}

void RealTimeTranslator::selectRoi(bool clicked)
{
	auto windowRect = m_watcher.getWindowSize();
	RECT emptyRect;
	emptyRect.left = 0;
	emptyRect.right = 0;
	emptyRect.top = 0;
	emptyRect.bottom = 0;
	PIX* img = m_watcher.capture(emptyRect);
	//m_canvas.showCanvas(windowRect);
	auto canvas = new InvisibleCanvas(this->m_roi);
	canvas->showCanvas(img, windowRect);
	canvas->show();
}
