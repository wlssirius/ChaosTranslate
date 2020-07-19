#include <QtWidgets/QMainWindow>
#include "ui_RealTimeTranslator.h"
#include "qpushbutton.h"
#include "qtextedit.h"
#include "qonlinetranslator.h"
#include "qrubberband.h"
#include "qcheckbox.h"
#include "qcombobox.h"
#include "QTranslator"
#include "ApplicationWatcher.h"
#include "SelectionCanvas.h"
#include "GlossaryManager.h"
#include "QString"
#include "memory"

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

	bool usingROI() const;
	bool usingFontColor() const;

public slots:
	void setSourceLanguage(int idx);
	void setTargetLanguage(int idx);

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

	Ui::RealTimeTranslatorClass ui;
	QPushButton* m_selectAppButton = nullptr;
	QPushButton* m_captureButton = nullptr;
	QPushButton* m_roiButton = nullptr;
	QPushButton* m_fontColorButton = nullptr;
	QPushButton* m_translateButton = nullptr;
	QPushButton* m_glossaryButton = nullptr;
	QPushButton* m_showImageButton = nullptr;
	QTextEdit* m_originalTextEdit = nullptr;
	QTextEdit* m_translateTextEdit = nullptr;
	QCheckBox* m_fontColorCheckBox = nullptr;
	QCheckBox* m_roiCheckBox = nullptr;
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
};
