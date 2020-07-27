#include "ChaosTranslate.h"
#include <qpushbutton.h>
#include "characterrecognize.h"
#include "qdialog.h"
#include "QtConcurrent/QtConcurrent"
#include "AppSelectDialog.h"
#include "LanguageMapping.h"

ChaosTranslate::ChaosTranslate(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	createLanguageMenu();

	m_selectAppButton = findChild<QPushButton*>("selectAppButton");
	connect(m_selectAppButton, &QPushButton::clicked, this, &ChaosTranslate::selectApp);
	m_captureButton = findChild<QPushButton*>("captureButton");
	connect(m_captureButton, &QPushButton::clicked, this,
		[this](bool clicked) { QtConcurrent::run(this, &ChaosTranslate::captureAndTranslate, clicked); });
	m_roiButton = findChild<QPushButton*>("ROIButton");
	connect(m_roiButton, &QPushButton::clicked, this, &ChaosTranslate::selectRoi);
	m_translateButton = findChild<QPushButton*>("translateButton");
	connect(m_translateButton, &QPushButton::clicked, this, &ChaosTranslate::translate);
	m_glossaryButton = findChild<QPushButton*>("glossaryButton");
	//m_glossaryButton->setHidden(true);
	connect(m_glossaryButton, &QPushButton::clicked, &m_glossary, &GlossaryManager::showDialog);
	m_fontColorButton = findChild<QPushButton*>("fontColorButton");
	connect(m_fontColorButton, &QPushButton::clicked, this, &ChaosTranslate::selectFontColor);
	m_originalTextEdit = findChild<QTextEdit*>("textEdit");
	connect(this, &ChaosTranslate::setOriginalText, m_originalTextEdit, &QTextEdit::setText);
	m_translateTextEdit = findChild<QTextEdit*>("textEdit_2");
	connect(this, &ChaosTranslate::setTranslateText, m_translateTextEdit, &QTextEdit::setText);
	m_fontColorCheckBox = findChild<QCheckBox*>("fontColorCheckBox");
	m_roiCheckBox = findChild<QCheckBox*>("roiCheckBox");
	m_srcLanguageComboBox = findChild<QComboBox*>("srcLanguageComboBox");
	m_tgtLanguageComboBox = findChild<QComboBox*>("tgtLanguageComboBox");
	for (int idx = 0; idx <= QOnlineTranslator::Language::Zulu; idx++)
	{
		QString language = QVariant::fromValue(QOnlineTranslator::Language(idx)).toString();
		m_srcLanguageComboBox->addItem(language);
		m_tgtLanguageComboBox->addItem(language);
	}
	m_srcLanguageComboBox->setCurrentIndex(m_sourceLanguage);
	m_tgtLanguageComboBox->setCurrentIndex(m_targetLanguage);
	connect(m_srcLanguageComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSourceLanguage(int)));
	connect(m_tgtLanguageComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setTargetLanguage(int)));

	m_roi.left = 0;
	m_roi.right = 0;
	m_roi.top = 0;
	m_roi.bottom = 0;

	connect(this, &ChaosTranslate::beginTranslate, this, &ChaosTranslate::translate);
}

void ChaosTranslate::selectApp(bool clicked)
{
	auto appList = m_watcher.getAppInfoList();
	auto appSelectDialog = new AppSelectDialog(appList);
	appSelectDialog->show();
	connect(appSelectDialog, &AppSelectDialog::selectApp, this, [this](QString str) {this->m_watcher.setApplication(str); });
}

void ChaosTranslate::captureAndTranslate(bool clicked)
{
	if (!m_watcher.appSelected())
	{
		return;
	}

	std::shared_ptr<PIX> pix(m_watcher.capture(m_roi));

	pixWrite("capture.png", pix.get(), IFF_PNG);

	BOX roi ;
	roi.x = m_roi.left;
	roi.y = m_roi.top;
	roi.w = m_roi.right-m_roi.left;
	roi.h = m_roi.bottom - m_roi.top;
	if (usingROI())
	{
		*pix = *pixClipRectangle(pix.get(), &roi, NULL);
	}
	m_capturedImage = convertPixToQImage(pix);
	pixWrite("capture.png", pix.get(), IFF_PNG);
	emit setOriginalText("Recognizing");
	if (m_fontColorCheckBox->isChecked())
	{
		thresholdByFontColor(pix.get());
	}
	QString language = languageMapping::qtToTesseract[m_sourceLanguage];
	QString capture = ocr(pix.get(), language);
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
}

void ChaosTranslate::translate(bool clicked)
{
	QString original = m_originalTextEdit->toPlainText();
	using LanguagePair = std::pair<QOnlineTranslator::Language, QOnlineTranslator::Language>;
	LanguagePair languagePair = LanguagePair(m_sourceLanguage, m_targetLanguage);
	GlossaryManager::EncodeResult encodeResult = m_glossary.encode(original, languagePair);
	emit setTranslateText("Translating");
	m_translator.translate(encodeResult.encodedText, QOnlineTranslator::Google, m_targetLanguage);
	QObject::connect(&m_translator, &QOnlineTranslator::finished, [=] {
		if (this->m_translator.error() == QOnlineTranslator::NoError)
		{
			auto translation = this->m_translator.translation();
			auto decoded = m_glossary.decode(translation, encodeResult.dictionary);
			emit setTranslateText(decoded);
		}
		else
		{
			emit setTranslateText(this->m_translator.errorString());
		}
		});
}

void ChaosTranslate::selectRoi(bool clicked)
{
	auto windowRect = m_watcher.getWindowSize();
	RECT emptyRect;
	emptyRect.left = 0;
	emptyRect.right = 0;
	emptyRect.top = 0;
	emptyRect.bottom = 0;
	std::shared_ptr<PIX> img(m_watcher.capture(emptyRect));
	auto qImg = convertPixToQImage(img);
	auto canvas = new SelectionCanvas(SelectionCanvas::Mode::ROI);
	connect(canvas, &SelectionCanvas::setROI, this, [this](RECT rect) {this->m_roi = rect; });
	canvas->showCanvas(qImg, windowRect);
}

void ChaosTranslate::selectFontColor(bool clicked)
{
	auto windowRect = m_watcher.getWindowSize();
	RECT emptyRect;
	emptyRect.left = 0;
	emptyRect.right = 0;
	emptyRect.top = 0;
	emptyRect.bottom = 0;
	std::shared_ptr<PIX> img(m_watcher.capture(emptyRect));
	auto qImg = convertPixToQImage(img);
	auto canvas = new SelectionCanvas(SelectionCanvas::Mode::Color);
	connect(canvas, &SelectionCanvas::setColor, this, [this](QColor color) {this->m_color = color; });
	canvas->showCanvas(qImg, windowRect);
}

bool ChaosTranslate::usingROI() const
{
	if (m_roiCheckBox != nullptr)
	{
		return m_roiCheckBox->isChecked();
	}
	return false;
}

bool ChaosTranslate::usingFontColor() const
{
	if (m_fontColorCheckBox != nullptr)
	{
		return m_fontColorCheckBox->isChecked();
	}
	return false;
}

void ChaosTranslate::setSourceLanguage(int idx)
{
	m_sourceLanguage = QOnlineTranslator::Language(idx);
	m_glossary.setSourceLanguage(m_sourceLanguage);
}

void ChaosTranslate::setTargetLanguage(int idx)
{
	m_targetLanguage = QOnlineTranslator::Language(idx);
	m_glossary.setTargetLanguage(m_targetLanguage);
}


void ChaosTranslate::thresholdByFontColor(PIX* pix)
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

void ChaosTranslate::switchTranslator(QTranslator& translator, const QString& filename)
{
	// remove the old translator
	qApp->removeTranslator(&translator);
	 
	// load the new translator
	QString path = QApplication::applicationDirPath();
	path.append("/");
	if (translator.load(path + filename)) //Here Path and Filename has to be entered because the system didn't find the QM Files else
		qApp->installTranslator(&translator);
}

void ChaosTranslate::loadLanguage(const QString& rLanguage)
{
	if (m_currUILang != rLanguage) {
		m_currUILang = rLanguage;
		QLocale locale = QLocale(m_currUILang);
		QLocale::setDefault(locale);
		QString languageName = QLocale::languageToString(locale.language());
		switchTranslator(m_uiTranslator, QString("realtimetranslator_%1.qm").arg(rLanguage));
		//ui.statusBar->showMessage(tr("Current Language changed to %1").arg(languageName));
	}
}

void ChaosTranslate::createLanguageMenu(void)
{
	QActionGroup* langGroup = new QActionGroup(ui.menuLanguage);
	langGroup->setExclusive(true);

	connect(langGroup, SIGNAL(triggered(QAction*)), this, SLOT(slotLanguageChanged(QAction*)));

	// format systems language
	QString defaultLocale = QLocale::system().name(); // e.g. "de_DE"
	defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "de"

	QDir dir("");
	QStringList fileNames = dir.entryList(QStringList("realtimetranslator_*.qm"));

	for (int i = 0; i < fileNames.size(); ++i) {
		// get locale extracted by filename
		QString locale;
		locale = fileNames[i]; // "TranslationExample_de.qm"
		locale.truncate(locale.lastIndexOf('.')); // "TranslationExample_de"
		locale.remove(0, locale.indexOf('_') + 1); // "de"

		QString lang = QLocale::languageToString(QLocale(locale).language());

		QAction* action = new QAction(lang, this);
		action->setCheckable(true);
		action->setData(locale);

		ui.menuLanguage->addAction(action);
		langGroup->addAction(action);

		// set default translators and language checked
		if (defaultLocale == locale)
		{
			action->setChecked(true);
			slotLanguageChanged(action);
		}
	}
}

std::shared_ptr<QImage> ChaosTranslate::convertPixToQImage(std::shared_ptr<PIX>& pix)
{
	std::shared_ptr<QImage> result = std::make_shared<QImage>(pix->w, pix->h, QImage::Format_ARGB32);
	for (int y = 0; y < pix->h; y++)
	{
		QRgb* destrow = (QRgb*)result->scanLine(y);
		for (int x = 0; x < pix->w; x++)
		{
			l_int32 r = 0;
			l_int32 g = 0;
			l_int32 b = 0;
			pixGetRGBPixel(pix.get(), x, y, &r, &g, &b);
			destrow[x] = qRgba(r, g, b, 255);
		}
	}

	return result;
}


void ChaosTranslate::changeEvent(QEvent* event)
{
	if (0 != event) {
		switch (event->type()) {
			// this event is send if a translator is loaded
		case QEvent::LanguageChange:
			ui.retranslateUi(this);
			break;

			// this event is send, if the system, language changes
		case QEvent::LocaleChange:
		{
			QString locale = QLocale::system().name();
			locale.truncate(locale.lastIndexOf('_'));
			loadLanguage(locale);
		}
		break;
		}
	}
	QMainWindow::changeEvent(event);
}

void ChaosTranslate::slotLanguageChanged(QAction* action)
{
	if (0 != action) {
		// load the language dependant on the action content
		loadLanguage(action->data().toString());
	}
}