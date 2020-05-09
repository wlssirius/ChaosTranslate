#include "RealTimeTranslator.h"
#include <qpushbutton.h>
#include "characterrecognize.h"
#include "qdialog.h"
#include "QtConcurrent/QtConcurrent"

RealTimeTranslator::RealTimeTranslator(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_captureButton = findChild<QPushButton*>("pushButton");

	connect(m_captureButton, &QPushButton::clicked, this,
		[this](bool clicked) { QtConcurrent::run(this, &RealTimeTranslator::captureAndTranslate, clicked); });
	m_roiButton = findChild<QPushButton*>("pushButton_2");
	connect(m_roiButton, &QPushButton::clicked, this, &RealTimeTranslator::selectRoi);
	m_translateButton = findChild<QPushButton*>("pushButton_3");
	connect(m_translateButton, &QPushButton::clicked, this, &RealTimeTranslator::translate);
	m_glossaryButton = findChild<QPushButton*>("pushButton_4");
	m_glossaryButton->setHidden(true);
	connect(m_glossaryButton, &QPushButton::clicked, &m_glossary, &GlossaryManager::showDialog);
	m_fontColorButton = findChild<QPushButton*>("pushButton_5");
	connect(m_fontColorButton, &QPushButton::clicked, this, &RealTimeTranslator::selectFontColor);
	m_originalTextEdit = findChild<QTextEdit*>("textEdit");
	connect(this, &RealTimeTranslator::setOriginalText, m_originalTextEdit, &QTextEdit::setText);
	m_translateTextEdit = findChild<QTextEdit*>("textEdit_2");
	connect(this, &RealTimeTranslator::setTranslateText, m_translateTextEdit, &QTextEdit::setText);
	m_fontColorCheckBox = findChild<QCheckBox*>("checkBox");

	m_roi.left = 0;
	m_roi.right = 0;
	m_roi.top = 0;
	m_roi.bottom = 0;

	connect(this, &RealTimeTranslator::beginTranslate, this, &RealTimeTranslator::translate);

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
	PIX* processed;
	if (roi->w == 0 || roi->h == 0)
	{
		processed = pix;
	}
	else
	{
		processed = pixClipRectangle(pix, roi, NULL);
	}
	emit setOriginalText("Recognizing");
	if (m_fontColorCheckBox->isChecked())
	{
		pixWrite("D:/TestFile_Pre_Process.png", processed, IFF_PNG);
		thresholdByFontColor(processed);
		pixWrite("D:/TestFile_Post_Process.png", processed, IFF_PNG);
	}
	QString capture = ocr(processed);
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
	emit setOriginalText(simplified);
	emit beginTranslate(true);
	//translate(true);
}

void RealTimeTranslator::translate(bool clicked)
{
	QString original = m_originalTextEdit->toPlainText();
	std::map<QString, QString> dict;
	auto encoded = m_glossary.encode(original, dict);
	emit setTranslateText("Translating");
	m_translator.translate(encoded, QOnlineTranslator::Google, QOnlineTranslator::SimplifiedChinese);
	QObject::connect(&m_translator, &QOnlineTranslator::finished, [=] {
		if (this->m_translator.error() == QOnlineTranslator::NoError)
		{
			auto translation = this->m_translator.translation();
			auto decoded = m_glossary.decode(translation, dict);
			emit setTranslateText(decoded);
		}
		else
		{
			emit setTranslateText(this->m_translator.errorString());
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
	////m_canvas.showCanvas(windowRect);
	//auto canvas = new InvisibleCanvas(InvisibleCanvas::Mode::ROI);
	//connect(canvas, &InvisibleCanvas::setROI, this, [this](RECT rect) {this->m_roi = rect; });
	//canvas->showCanvas(img, windowRect);
	//canvas->show();
}

void RealTimeTranslator::selectFontColor(bool clicked)
{
	auto windowRect = m_watcher.getWindowSize();
	RECT emptyRect;
	emptyRect.left = 0;
	emptyRect.right = 0;
	emptyRect.top = 0;
	emptyRect.bottom = 0;
	PIX* img = m_watcher.capture(emptyRect);
	auto canvas = new InvisibleCanvas(InvisibleCanvas::Mode::Color);
	connect(canvas, &InvisibleCanvas::setColor, this, [this](QColor color) {this->m_color = color; });
	canvas->showCanvas(img, windowRect);
	canvas->show();
}


void RealTimeTranslator::thresholdByFontColor(PIX* pix)
{
	unsigned char r1 = m_color.redF() * 255;
	unsigned char g1 = m_color.greenF() * 255;
	unsigned char b1 = m_color.blueF() * 255;
	const auto isSimilarColor = [=](l_int32 r, l_int32 g, l_int32 b)
	{
		if ((r - r1) * (r - r1) + (g - g1) * (g - g1) + (b - b1) * (b - b1) <= 1000)
		{
			return true;
		}
		else
		{
			return false;
		}
	};
	for (int i = 0; i < pix->w; i++)
	{
		for (int j = 0; j < pix->h; j++)
		{
			l_int32 r = 0;
			l_int32 g = 0;
			l_int32 b = 0;
			pixGetRGBPixel(pix, i, j, &r, &g, &b);
			if (isSimilarColor(r, g, b))
			{
				pixSetRGBPixel(pix, i, j, 255, 255, 255);
			}
			else
			{
				pixSetRGBPixel(pix, i, j, 0, 0, 0);
			}
		}
	}
}