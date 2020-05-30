#include <QtWidgets/QMainWindow>
#include "ui_RealTimeTranslator.h"
#include "qpushbutton.h"
#include "qtextedit.h"
#include "qonlinetranslator.h"
#include "qrubberband.h"
#include "qcheckbox.h"
#include "qcombobox.h"
#include "ApplicationWatcher.h"
#include "InvisibleCanvas.h"
#include "GlossaryManager.h"
#include "QString"

class RealTimeTranslator : public QMainWindow
{
	Q_OBJECT

public:
	RealTimeTranslator(QWidget *parent = Q_NULLPTR);

	void selectApp(bool clicked);
	void captureAndTranslate(bool clicked);
	void translate(bool clicked);
	void selectRoi(bool clicked);
	void selectFontColor(bool clicked);
	void setRoi(RECT roi) { m_roi = roi; }

public slots:
	void setSourceLanguage(int idx);
	void setTargetLanguage(int idx);

signals:
	void setOriginalText(const QString& text);
	void setTranslateText(const QString& text);
	void beginTranslate(bool clicked);

private:
	void thresholdByFontColor(PIX* pix);

	Ui::RealTimeTranslatorClass ui;
	QPushButton* m_selectAppButton;
	QPushButton* m_captureButton;
	QPushButton* m_roiButton;
	QPushButton* m_fontColorButton;
	QPushButton* m_translateButton;
	QPushButton* m_glossaryButton;
	QTextEdit* m_originalTextEdit;
	QTextEdit* m_translateTextEdit;
	QCheckBox* m_fontColorCheckBox;
	QComboBox* m_srcLanguageComboBox;
	QComboBox* m_tgtLanguageComboBox;

	QOnlineTranslator m_translator;
	QOnlineTranslator::Language m_sourceLanguage = QOnlineTranslator::Language::Japanese;
	QOnlineTranslator::Language m_targetLanguage = QOnlineTranslator::Language::SimplifiedChinese;

	ApplicationWatcher m_watcher;
	GlossaryManager m_glossary;
	RECT m_roi;
	QColor m_color;
	QString m_appTitle;
};
