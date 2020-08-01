#include <QtWidgets/QMainWindow>
#include "ui_ChaosTranslate.h"
#include "qpushbutton.h"
#include "qtextedit.h"
#include "qonlinetranslator.h"
#include "qrubberband.h"
#include "qcheckbox.h"
#include "qcombobox.h"
#include "QRadioButton"
#include "QTranslator"
#include "ApplicationWatcher.h"
#include "SelectionCanvas.h"
#include "GlossaryManager.h"
#include "QString"
#include "QColorPicker.h"
#include "memory"

class ChaosTranslate : public QMainWindow
{
	Q_OBJECT

public:
	ChaosTranslate(QWidget *parent = Q_NULLPTR);

	void selectApp(bool clicked);
	void captureAndTranslate(bool clicked);
	void translate(bool clicked);
	void selectRoi(bool clicked);
	void selectFontColor(bool clicked);
	void setRoi(RECT roi) { m_roi = roi; }

public slots:
	void setSourceLanguage(int idx);
	void setTargetLanguage(int idx);
	void setEntireAppCapture(bool clicked);
	void setRegionCapture(bool clicked);
	void setAutoDetectFontColor(bool clicked);
	void setManualChooseFontColor(bool clicked);

signals:
	void setOriginalText(const QString& text);
	void setTranslateText(const QString& text);
	void beginTranslate(bool clicked);

protected:
	void changeEvent(QEvent* event);
protected slots:
	void slotLanguageChanged(QAction* action);

private:
	void thresholdByFontColor(PIX* pix);

	void switchTranslator(QTranslator& translator, const QString& filename);
	void loadLanguage(const QString& rLanguage);
	void createLanguageMenu(void);
	std::shared_ptr<QImage> convertPixToQImage(std::shared_ptr<PIX>& pix);

	Ui::ChaosTranslateClass ui;
	QPushButton* m_selectAppButton = nullptr;
	QPushButton* m_captureButton = nullptr;
	QPushButton* m_roiButton = nullptr;
	QColorPicker* m_fontColorButton = nullptr;
	QPushButton* m_translateButton = nullptr;
	QPushButton* m_glossaryButton = nullptr;
	QLabel* m_imageLabel = nullptr;
	QTextEdit* m_originalTextEdit = nullptr;
	QTextEdit* m_translateTextEdit = nullptr;
	QRadioButton* m_entireAppRadioButton = nullptr;
	QRadioButton* m_regionRadioButton = nullptr;
	QRadioButton* m_autoColorRadioButton = nullptr;
	QRadioButton* m_setColorRadioButton = nullptr;
	QComboBox* m_srcLanguageComboBox = nullptr;
	QComboBox* m_tgtLanguageComboBox = nullptr;

	QOnlineTranslator m_translator;
	QOnlineTranslator::Language m_sourceLanguage = QOnlineTranslator::Language::Japanese;
	QOnlineTranslator::Language m_targetLanguage = QOnlineTranslator::Language::SimplifiedChinese;

	QTranslator m_uiTranslator;
	QString m_currUILang;

	ApplicationWatcher m_watcher;
	GlossaryManager m_glossary;
	RECT m_roi;
	QColor m_color;
	QString m_appTitle;
	std::shared_ptr<QImage> m_capturedImage;

	bool m_regionalCapture = false;
	bool m_manualSetFontColor = false;
};
